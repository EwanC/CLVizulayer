// Copyright (c) 2025 Ewan Crawford

#include "viz_instance.h"
#include "io.h"
#include "logger.h"
#include "viz_context.h"
#include "viz_node.h"
#include "viz_queue.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <stdexcept>

VizInstance::VizInstance(bool Color, const char *path) {
  MDotFile = std::make_unique<VizDotFile>(Color, path);
}

VizInstance::~VizInstance() {
  // Flush unprinted nodes to dot file before tearing down resources
  std::stack<VizNode *> LeafStack;
  for (VizQueue *VQ : MQueues) {
    auto &Leaves = VQ->MLeaves;
    for (auto &Leaf : Leaves) {
      LeafStack.push(Leaf);
    }
  }
  flush(LeafStack, "Viz instance teardown");

  for (VizQueue *VQ : MQueues) {
    delete VQ;
  }

  for (VizNode *VN : MNodes) {
    delete VN;
  }

  VIZ_LOG("VizInstance {} destroyed", static_cast<void *>(this));
}

void VizInstance::createVizQueue(cl_command_queue CQ, bool IsInOrder) {
  VizQueue *VQ = new VizQueue(CQ, IsInOrder);
  MQueues.push_back(VQ);

  // An OpenCL implementation may reuse a pointer for a command-queue
  // from a previously released command-queue.
  auto Itr = MQueueMap.find(CQ);
  if (Itr != MQueueMap.end()) {
    MQueueMap.erase(Itr);
    VIZ_LOG("Instance {} reusing CL queue {} for new Viz queue {}",
            static_cast<void *>(this), static_cast<void *>(CQ),
            static_cast<void *>(VQ));
  }

  MQueueMap.emplace(CQ, VQ);

  VIZ_LOG("Instance {} created Viz queue {} for CL queue {}, in-order: {}",
          static_cast<void *>(this), static_cast<void *>(VQ),
          static_cast<void *>(CQ), IsInOrder);
}

void VizInstance::createVizNode(
    cl_command_queue CQ, const char *Name,
    std::function<void(std::ofstream &)> VerbosePrintFunc,
    std::span<const cl_event> Deps, cl_event *RetEvent) {
  VizQueue *VQ = getVizQueueForCLQueue(CQ);
  if (!VQ) {
    return;
  }
  std::vector<VizNode *> DepVizNodes;
  NodePreCreation(VQ, Deps, DepVizNodes);
  VizNode *Node = new VizNode(VQ, std::move(DepVizNodes), Name,
                              std::move(VerbosePrintFunc));
  NodePostCreation(Node, VQ, RetEvent);

  VIZ_LOG("Instance {} created node {}: CL queue {}, Viz queue {}",
          static_cast<void *>(this), Name, static_cast<void *>(CQ),
          static_cast<void *>(VQ));
}

void VizInstance::createVizMarkerNode(cl_command_queue CQ,
                                      std::span<const cl_event> Deps,
                                      cl_event *RetEvent) {
  VizQueue *VQ = getVizQueueForCLQueue(CQ);
  if (!VQ) {
    return;
  }
  std::vector<VizNode *> DepVizNodes;
  NodePreCreation(VQ, Deps, DepVizNodes, true);

  auto &Leaves = VQ->MLeaves;
  if (Deps.empty()) {
    for (auto &Leaf : Leaves) {
      DepVizNodes.push_back(Leaf);
    }
  }

  // Remove dependencies from list of leaf nodes
  for (VizNode *Dep : DepVizNodes) {
    Leaves.erase(std::remove(Leaves.begin(), Leaves.end(), Dep), Leaves.end());
  }

  VizNode *Node = new VizNode(VQ, std::move(DepVizNodes),
                              "clEnqueueMarkerWithWaitList", {});

  NodePostCreation(Node, VQ, RetEvent);

  VIZ_LOG("Instance {} Marker node added: Queue - {}",
          static_cast<void *>(this), static_cast<void *>(CQ));
}

void VizInstance::createVizMarkerNode(cl_command_queue CQ, cl_event *RetEvent) {
  VizQueue *VQ = getVizQueueForCLQueue(CQ);
  if (!VQ) {
    return;
  }
  std::vector<VizNode *> DepVizNodes;
  std::span<const cl_event> Deps{};
  NodePreCreation(VQ, Deps, DepVizNodes, true);

  auto &Leaves = VQ->MLeaves;
  for (auto &Leaf : Leaves) {
    DepVizNodes.push_back(Leaf);
  }

  // Remove dependencies from list of leaf nodes
  for (VizNode *Dep : DepVizNodes) {
    Leaves.erase(std::remove(Leaves.begin(), Leaves.end(), Dep), Leaves.end());
  }

  VizNode *Node =
      new VizNode(VQ, std::move(DepVizNodes), "clEnqueueMarker", {});

  NodePostCreation(Node, VQ, RetEvent);

  VIZ_LOG("Instance {} Marker node added: Queue - {}",
          static_cast<void *>(this), static_cast<void *>(CQ));
}

void VizInstance::createVizBarrierNode(cl_command_queue CQ, bool HasWaitList,
                                       std::span<const cl_event> Deps,
                                       cl_event *RetEvent) {
  VizQueue *VQ = getVizQueueForCLQueue(CQ);
  if (!VQ) {
    return;
  }
  std::vector<VizNode *> DepVizNodes;
  NodePreCreation(VQ, Deps, DepVizNodes, true);

  auto &Leaves = VQ->MLeaves;
  if (Deps.empty()) {
    for (auto &Leaf : Leaves) {
      DepVizNodes.push_back(Leaf);
    }
  }

  // Remove dependencies from list of leaf nodes
  for (VizNode *Dep : DepVizNodes) {
    Leaves.erase(std::remove(Leaves.begin(), Leaves.end(), Dep), Leaves.end());
  }

  const char *Type =
      HasWaitList ? "clEnqueueBarrierWithWaitList" : "clEnqueueBarrier";
  VizNode *Node = new VizNode(VQ, std::move(DepVizNodes), Type, {});
  NodePostCreation(Node, VQ, RetEvent);

  VQ->MLastBarrier = Node;

  VIZ_LOG("Instance {} added node {}: Queue - {}", static_cast<void *>(this),
          Type, static_cast<void *>(CQ));
}

void VizInstance::flushEvents(std::span<const cl_event> Events) {
  std::stack<VizNode *> NodeStack;
  for (auto &E : Events) {
    // When using the fine grained extension, the commands associated with
    // events before capture began may not be associated with nodes
    auto Itr = MEventMap.find(E);
    if (Itr != MEventMap.end()) {
      NodeStack.push(Itr->second);
    }
  }

  flush(NodeStack, "clWaitForEvents()");

#ifdef VIZ_DEBUG
  // Log events flushed when in debug printing mode
  for (auto &E : Events) {
    VIZ_LOG("Instance {} Event flushed: {}", static_cast<void *>(this),
            static_cast<void *>(E));
  }
#endif
}

void VizInstance::flushQueue(cl_command_queue CQ, const char *Label,
                             bool IsBlockingCommand) {
  VizQueue *VQ = getVizQueueForCLQueue(CQ);
  if (!VQ) {
    return;
  }

  std::stack<VizNode *> NodeStack;
  if (!IsBlockingCommand) {
    // From clReleaseCommandQueue() or clFinish() - find all leaves
    auto &Leaves = VQ->MLeaves;
    for (auto &Leaf : Leaves) {
      NodeStack.push(Leaf);
    }
  } else {
    // Called after blocking command is created, which will be last node
    NodeStack.push(MNodes.back());
  }

  flush(NodeStack, Label);
  VIZ_LOG("Instance {} flushed queue {} from command {}",
          static_cast<void *>(this), static_cast<void *>(CQ), Label);
}

void VizInstance::flush(std::stack<VizNode *> &NodeStack, const char *Label) {
  std::set<VizNode *, decltype(&vizNodeCmp)> printNodes(&vizNodeCmp);
  while (!NodeStack.empty()) {
    auto Node = NodeStack.top();
    NodeStack.pop();

    if (!Node->MDefined) {
      printNodes.insert(Node);

      for (auto &Pred : Node->MPreds) {
        NodeStack.push(Pred);
      }
    }
  }

  MDotFile->writeSubgraph(printNodes, Label);
}

VizQueue *VizInstance::getVizQueueForCLQueue(cl_command_queue CQ) const {
  // If using the fine grained extension, then not all CL queues
  // may be getting tracked for recording.
  auto &Context = getVizContext();
  if (Context.useExt()) {
    auto VQItr = MQueueMap.find(CQ);
    if (VQItr == MQueueMap.end()) {
      VIZ_LOG("Instance {} couldn't find Viz queue for CL queue {}",
              static_cast<const void *>(this), static_cast<void *>(CQ));
      return nullptr;
    }
    return VQItr->second;
  } else {
    return MQueueMap.at(CQ);
  }
}

void VizInstance::NodePostCreation(VizNode *Node, VizQueue *VQ,
                                   cl_event *RetEvent) {
  MNodes.push_back(Node);
  if (RetEvent) {
    // An OpenCL implementation may reuse a pointer for an event
    // from a previously released event
    auto Itr = MEventMap.find(*RetEvent);
    if (Itr != MEventMap.end()) {
      MEventMap.erase(Itr);
    }

    MEventMap.emplace(*RetEvent, Node);
  }

  // Set new in-order queue last node
  if (VQ->MIsInOrder) {
    VQ->MLastNode = Node;
  }

  // Set new leaf node
  VQ->MLeaves.push_back(Node);
}

void VizInstance::NodePreCreation(VizQueue *VQ, std::span<const cl_event> Deps,
                                  std::vector<VizNode *> &DepVizNodes,
                                  bool IsBarrierOrMarker) {
  // Get list of explicitly passed node dependencies
  for (cl_event E : Deps) {
    // When using the fine grained extension, the commands associated with
    // events before capture began may not be associated with nodes
    auto Itr = MEventMap.find(E);
    if (Itr != MEventMap.end()) {
      DepVizNodes.push_back(Itr->second);
    } else {
      VIZ_LOG("Instance {} couldn't find node for CL event {}",
              static_cast<void *>(this), static_cast<void *>(E));
    }
  }

  // Get in-order queue dependency
  if (VQ->MLastNode) {
    DepVizNodes.push_back(VQ->MLastNode);
  }

  // Get any barrier that has been used
  if (VQ->MLastBarrier) {
    DepVizNodes.push_back(VQ->MLastBarrier);
  }

  if (!IsBarrierOrMarker) {
    // Remove dependencies from list of leaf nodes
    auto &Leaves = VQ->MLeaves;
    for (VizNode *Dep : DepVizNodes) {
      Leaves.erase(std::remove(Leaves.begin(), Leaves.end(), Dep),
                   Leaves.end());
    }
  }
}
