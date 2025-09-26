// Copyright (c) 2025 Ewan Crawford
#pragma once

#include <CL/cl_layer.h>
#include <array>
#include <functional>
#include <memory>
#include <mutex>
#include <span>
#include <stack>
#include <unordered_map>
#include <vector>

/// See main.cpp for initalization
extern const struct _cl_icd_dispatch *TargetDispatch;

/// Forward declarations
struct VizNode;
struct VizDotFile;

/// @brief Tracks scheduling information relating to a command-queue
struct VizQueue {
  /// @brief Constructor
  VizQueue(cl_command_queue Queue, bool IsInOrder)
      : MQueue(Queue), MIsInOrder(IsInOrder), MLastNode(nullptr),
        MLastBarrier(nullptr) {}

  /// Command-queue being tracked
  cl_command_queue MQueue;
  /// Whether the command-queue is in-order or out-of-order
  bool MIsInOrder;
  /// The last node enqueued to @a MQueue, nullptr if @a MIsInOrder is false.
  VizNode *MLastNode;
  /// The last barrier command enqueued to @a MQueue
  VizNode *MLastBarrier;
  /// Leaf nodes of the queue
  std::vector<VizNode *> MLeaves;
};

/// @brief Represents a graph node
struct VizNode {
  /// @brief Constructor
  VizNode(VizQueue *VQ, std::vector<VizNode *> &&Deps, const char *Name,
          std::function<void(std::ofstream &)> VerbosePrint)
      : MQueue(VQ), MPreds(Deps), MName(Name), MVerbosePrint(VerbosePrint),
        MDefined(false) {
    static int ID = 0;
    MID = ID++;
  }

  /// Queue which the node belongs to
  VizQueue *MQueue;
  /// Dependencies, or predecessors, of the node
  const std::vector<VizNode *> MPreds;
  /// Name of command, e.g "clEnqueueNDRangeKernel"
  const char *MName;
  /// Optional function for printing verbose node information
  std::function<void(std::ofstream &)> MVerbosePrint;
  /// Whether the node has already been defined in a dot file
  bool MDefined;
  /// Unique ID for the node
  unsigned MID;
};

/// @brief Context holding the state of the layer
struct VizInstance {
  /// @brief Constructor
  /// @param[in] Color true if color environment variable set, false otherwise
  /// @param[in] FilePath User defined path for the DOT file, or nullptr for
  /// layer default.
  VizInstance(bool Color, const char *FilePath = nullptr);

  /// @brief Destructor
  ~VizInstance();

  /// @brief Allocates a new VizQueue instance and stores in @a MQueues
  /// @param[in] CQ OpenCL command-queue
  /// @param[in] IsInOrder True if @a CQ is in-order, False if out-of-order
  void createVizQueue(cl_command_queue CQ, bool IsInOrder);

  /// @brief Allocates a new VizNode instance and stores it in @a MNodes
  /// @param[in] CQ OpenCL command-queue the node was created on
  /// @param[in] Name Entry-point name that created the node
  /// @param[in] VerbosePrintFunc Optional function for printing verbose node
  /// details.
  /// @param[in] Deps OpenCL event dependencies
  /// @param[out] RetEvent OpenCL return event
  void createVizNode(cl_command_queue CQ, const char *Name,
                     std::function<void(std::ofstream &)> VerbosePrintFunc,
                     std::span<const cl_event> Deps, cl_event *RetEvent);

  /// @brief Allocates a new VizNode instance representing a barrier command,
  /// and stores it in @a MNodes
  /// @param[in] CQ OpenCL command-queue the node was created on
  /// @param[in] HasWaitList True if clEnqueueBarrierWithWaitList, False if
  /// clEnqueueBarrier
  /// @param[in] Deps OpenCL event dependencies
  /// @param[out] RetEvent OpenCL return event
  void createVizBarrierNode(cl_command_queue CQ, bool HasWaitList,
                            std::span<const cl_event> Deps, cl_event *RetEvent);

  /// @brief Allocates a new VizNode instance representing a marker command,
  /// and stores it in @a MNodes
  /// @param[in] CQ OpenCL command-queue the node was created on
  /// @param[in] Deps OpenCL event dependencies
  /// @param[out] RetEvent OpenCL return event
  void createVizMarkerNode(cl_command_queue CQ, std::span<const cl_event> Deps,
                           cl_event *RetEvent);

  /// @brief Allocates a new VizNode instance representing a marker command,
  /// and stores it in @a MNodes
  /// @param[in] CQ OpenCL command-queue the node was created on
  /// @param[out] RetEvent OpenCL return event
  void createVizMarkerNode(cl_command_queue CQ, cl_event *RetEvent);

  /// @brief Write cached nodes for OpenCL events to dot file
  /// @param[in] Events The OpenCL events to print nodes for
  void flushEvents(std::span<const cl_event> Events);

  /// @brief Write cached nodes for command-queue to dot file
  /// @param[in] CQ Command-queue being flushed
  /// @param[in] Label Name of OpenCL entry-point that caused flush
  /// @param[in] IsBlockingCommand True if flush comes a user submitting a
  /// blocking device command, false if call came from clReleaseCommandQueue()
  /// or clFinish().
  void flushQueue(cl_command_queue CQ, const char *Label,
                  bool IsBlockingCommand);

private:
  /// @brief Write nodes and their dependencies to dot file
  /// @detail Nodes will be taken off the stack as they are printed, and
  /// dependencies will be added to the stack in a Breadth First manner.
  /// @param[in] NodeStack List of nodes to print. Will be empty at the end
  /// of a successful function call.
  /// @param[in] Label Name to give dot cluster nodes will be printed in
  void flush(std::stack<VizNode *> &NodeStack, const char *Label);

  /// @brief Finds the VizQueue object associated with a OpenCL queue.
  /// @param[in] CQ The OpenCL queue that a Viz queue has been created from.
  /// @throws RuntimeException If fine grain extension is not being used and
  /// there is no matching Viz queue owned by the Viz instance.
  /// @return Non-owning pointer to found Viz queue, or nullptr if fine grain
  /// extension is being used and no Viz queue was found.
  VizQueue *getVizQueueForCLQueue(cl_command_queue CQ) const;

  /// @brief Generic work before a node has been created
  /// param[in] VQ Queue the node will be created on
  /// param[in] Deps OpenCL events the node depends on
  /// param[out] DepVizNodes Viz nodes the node will depend on
  /// param[in] isBarrierOrMarker Whether the node being created is a
  /// barrier or a marker.
  void NodePreCreation(VizQueue *VQ, std::span<const cl_event> Deps,
                       std::vector<VizNode *> &DepVizNode,
                       bool isBarrierOrMarker = false);

  /// @brief Generic work after a node has been created
  /// @param[in] Node The node which has been created
  /// @param[in] VQ Queue the node was enqueued to
  /// @param[out] RetEvent The OpenCL event returned from node creation
  void NodePostCreation(VizNode *Node, VizQueue *VQ, cl_event *RetEvent);

  /// DOT file used for output
  std::unique_ptr<VizDotFile> MDotFile;

  /// Map of OpenCL queues to VIZ queue objects
  std::unordered_map<cl_command_queue, VizQueue *> MQueueMap;

  /// Map of OpenCL events to VIZ nodes
  std::unordered_map<cl_event, VizNode *> MEventMap;

  /// List of heap allocated Viz queue objects,
  /// must be freed on context destruction
  std::vector<VizQueue *> MQueues;

  /// List of heap allocated Viz node objects,
  /// must be freed on context destruction
  std::vector<VizNode *> MNodes;
};

/// @brief Context holding the state of the layer. Singleton instance of context
/// is created by the layer and used for the duration of the application.
struct VizContext {
  /// @brief Constructor
  VizContext();

  /// @brief Destructor
  ~VizContext();

  /// @brief Allocation a new VizInstance and stores it in @a MInstances
  /// @param[in] CLQueueList List of OpenCL queues to start tracking
  /// @param[in] FilePath User defined path for the DOT file, or nullptr for
  /// layer default.
  VizInstance *createVizInstance(std::span<const cl_command_queue> CLQueueList,
                                 const char *FilePath);

  /// @brief Frees heap allocated VizInstance and removes it from @a MInstances
  /// @param[in] VI Pointer to VizInstance object to free and erase.
  void destroyVizInstance(VizInstance *VI);

  /// @brief For each instance, allocates a new VizQueue instance.
  /// @param[in] CQ OpenCL command-queue
  /// @param[in] IsInOrder True if @a CQ is in-order, False if out-of-order
  void createVizQueue(cl_command_queue CQ, bool IsInOrder) {
    std::lock_guard<std::mutex> Lock(MMutex);
    for (auto &VI : MInstances) {
      VI->createVizQueue(CQ, IsInOrder);
    }
  }

  /// @brief For each instance, allocates a new VizNode instance.
  /// @param[in] CQ OpenCL command-queue the node was created on
  /// @param[in] Name Entry-point name that created the node
  /// @param[in] VerbosePrintFunc Optional function for printing verbose node
  /// details.
  /// @param[in] Deps OpenCL event dependencies
  /// @param[out] RetEvent OpenCL return event
  void createVizNode(cl_command_queue CQ, const char *Name,
                     std::function<void(std::ofstream &)> VerbosePrintFunc,
                     std::span<const cl_event> Deps, cl_event *RetEvent) {
    std::lock_guard<std::mutex> Lock(MMutex);
    for (auto &VI : MInstances) {
      VI->createVizNode(CQ, Name, VerbosePrintFunc, Deps, RetEvent);
    }
  }

  /// @brief For each instance, allocates a new VizNode instance representing a
  /// barrier command.
  /// @param[in] CQ OpenCL command-queue the node was created on
  /// @param[in] HasWaitList True if clEnqueueBarrierWithWaitList, False if
  /// clEnqueueBarrier
  /// @param[in] Deps OpenCL event dependencies
  /// @param[out] RetEvent OpenCL return event
  void createVizBarrierNode(cl_command_queue CQ, bool HasWaitList,
                            std::span<const cl_event> Deps,
                            cl_event *RetEvent) {
    std::lock_guard<std::mutex> Lock(MMutex);
    for (auto &VI : MInstances) {
      VI->createVizBarrierNode(CQ, HasWaitList, Deps, RetEvent);
    }
  }

  /// @brief For each instance, allocates a new VizNode instance representing a
  /// marker command.
  /// @param[in] CQ OpenCL command-queue the node was created on
  /// @param[in] Deps OpenCL event dependencies
  /// @param[out] RetEvent OpenCL return event
  void createVizMarkerNode(cl_command_queue CQ, std::span<const cl_event> Deps,
                           cl_event *RetEvent) {
    std::lock_guard<std::mutex> Lock(MMutex);
    for (auto &VI : MInstances) {
      VI->createVizMarkerNode(CQ, Deps, RetEvent);
    }
  }

  /// @brief For each instance, allocates a new VizNode instance representing a
  /// marker command.
  /// @param[in] CQ OpenCL command-queue the node was created on
  /// @param[out] RetEvent OpenCL return event
  void createVizMarkerNode(cl_command_queue CQ, cl_event *RetEvent) {
    std::lock_guard<std::mutex> Lock(MMutex);
    for (auto &VI : MInstances) {
      VI->createVizMarkerNode(CQ, RetEvent);
    }
  }

  /// @brief For each instance, writes cached nodes for OpenCL events to dot
  /// file
  /// @param[in] Events The OpenCL events to print nodes for
  void flushEvents(std::span<const cl_event> Events) {
    std::lock_guard<std::mutex> Lock(MMutex);
    for (auto &VI : MInstances) {
      VI->flushEvents(Events);
    }
  }

  /// @brief Write cached nodes for command-queue to dot file
  /// @param[in] CQ Command-queue being flushed
  /// @param[in] Label Name of OpenCL entry-point that caused flush
  /// @param[in] IsBlockingCommand True if flush comes a user submitting a
  /// blocking device command, false if call came from clReleaseCommandQueue()
  /// or clFinish().
  void flushQueue(cl_command_queue CQ, const char *Label,
                  bool IsBlockingCommand) {
    std::lock_guard<std::mutex> Lock(MMutex);
    for (auto &VI : MInstances) {
      VI->flushQueue(CQ, Label, IsBlockingCommand);
    }
  }

  /// @brief Query for if verbose environment variable is set
  bool verbose() const noexcept { return MVerbose; }

  /// @brief Query for if extension environment variable is set
  bool useExt() const noexcept { return MExt; }

private:
  /// Verbose environment variable set
  bool MVerbose;

  /// Color environment variable set
  bool MColor;

  /// Extension environment variable set
  bool MExt;

  /// List of heap allocated Viz instance objects,
  /// must be freed on context destruction
  std::vector<VizInstance *> MInstances;

  /// Only one thread can access the context at a time.
  std::mutex MMutex;
};

/// Get the singleton instance of the context
/// @return Reference to the context instance
VizContext &getVizContext();
