// Copyright (c) 2025 Ewan Crawford

#include "viz_context.h"
#include "logger.h"

#include <string>

VizContext &getVizContext() {
  static VizContext Context;
  return Context;
}

VizContext::VizContext() {
  MVerbose = false;
  if (char *EnvVerbose = std::getenv("VIZ_VERBOSE"); EnvVerbose) {
    if (0 != std::atoi(EnvVerbose)) {
      MVerbose = true;
      VIZ_LOG("VIZ_VERBOSE - Enabled");
    } else {
      VIZ_LOG("VIZ_VERBOSE - Disabled");
    }
  }

  MColor = false;
  if (char *EnvColor = std::getenv("VIZ_COLOR"); EnvColor) {
    if (0 != std::atoi(EnvColor)) {
      MColor = true;
      VIZ_LOG("VIZ_COLOR - Enabled");
    } else {
      VIZ_LOG("VIZ_COLOR - Disabled");
    }
  }

  MExt = false;
  if (char *EnvExt = std::getenv("VIZ_EXT");
      EnvExt && (0 != std::atoi(EnvExt))) {
    VIZ_LOG("VIZ_EXT - Enabled");
    MExt = true;
    // Wait for user to create `cl_dot_graph_ext` object that will initialize
    // VizInstance instances in the VizContext.
  } else {
    VIZ_LOG("VIZ_EXT - Disabled");
    VizInstance *VI = new VizInstance(MColor);
    VIZ_LOG("VizInstance {} Created", static_cast<void *>(VI));
    MInstances.push_back(VI);
  }
}

VizContext::~VizContext() {
  for (VizInstance *VI : MInstances) {
    delete VI;
  }
  MInstances.clear();

  for (auto &Itr : MCommandBufferInstanceMap) {
    delete Itr.second;
  }
  MCommandBufferInstanceMap.clear();

  VIZ_LOG("VizContext {} destroyed", static_cast<void *>(this));
}

VizInstance *
VizContext::createVizInstance(std::span<const cl_command_queue> CLQueueList,
                              const char *FilePath) {
  std::lock_guard<std::mutex> Lock(MMutex);
  VizInstance *VI = new VizInstance(MColor, FilePath);
  MInstances.push_back(VI);
  VIZ_LOG("VizInstance {} created", static_cast<void *>(VI));

  for (auto &CLQueue : CLQueueList) {
    cl_command_queue_properties QueueProps;
    auto Ret = TargetDispatch->clGetCommandQueueInfo(
        CLQueue, CL_QUEUE_PROPERTIES, sizeof(QueueProps), &QueueProps, NULL);
    if (Ret != CL_SUCCESS) {
      throw std::runtime_error(std::string(
          "OpenCL error querying queue properties " + std::to_string(Ret)));
    }

    VI->createVizQueue(
        CLQueue, (QueueProps & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) == 0);
  }

  return VI;
}

void VizContext::destroyVizInstance(VizInstance *VI) {
  std::lock_guard<std::mutex> Lock(MMutex);
  // Free heap data at pointer
  delete VI;

  // Move the VI element to the end of the list and return iterator
  // Erase pointer from std::vector
  MInstances.erase(std::remove(MInstances.begin(), MInstances.end(), VI),
                   MInstances.end());
}

void VizContext::flushCommandBuffer(
    cl_command_buffer_khr CB, const cl_command_buffer_dot_print_flags_ext Flags,
    const char *FilePath) {

  std::lock_guard<std::mutex> Lock(MMutex);
  auto Itr = MCommandBufferInstanceMap.find(CB);
  if (Itr != MCommandBufferInstanceMap.end()) {
    auto &VI = Itr->second;
    VI->flushCommandBuffer(Flags, FilePath);
  }
}

void VizContext::createVizInstance(cl_command_buffer_khr CB,
                                   cl_command_queue CQ) {
  std::lock_guard<std::mutex> Lock(MMutex);

  cl_command_queue_properties QueueProps;
  auto Ret = TargetDispatch->clGetCommandQueueInfo(
      CQ, CL_QUEUE_PROPERTIES, sizeof(QueueProps), &QueueProps, NULL);
  if (Ret != CL_SUCCESS) {
    throw std::runtime_error(std::string(
        "OpenCL error querying queue properties " + std::to_string(Ret)));
  }

  bool IsInOrder = (QueueProps & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) == 0;

  VizInstance *VI = new VizInstance(CB, IsInOrder);
  MCommandBufferInstanceMap.emplace(CB, VI);
  VIZ_LOG("VizInstance {} Created for CB {}", static_cast<void *>(VI),
          static_cast<void *>(CB));
}

void VizContext::destroyVizInstance(cl_command_buffer_khr CB) {
  std::lock_guard<std::mutex> Lock(MMutex);
  auto Itr = MCommandBufferInstanceMap.find(CB);
  if (Itr != MCommandBufferInstanceMap.end()) {
    auto &VI = Itr->second;
    delete VI; // Free heap allocated VIzInstance
    MCommandBufferInstanceMap.erase(Itr);

    VIZ_LOG("VizInstance {} destroyed for CB {}", static_cast<void *>(VI),
            static_cast<void *>(Itr->first));
  }
}
