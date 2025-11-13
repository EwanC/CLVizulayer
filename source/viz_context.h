// Copyright (c) 2025 Ewan Crawford
#pragma once

#define CL_ENABLE_BETA_EXTENSIONS

#include "viz_instance.h"
#include <CL/cl_ext.h>
#include <CL/cl_layer.h>
#include <cassert>
#include <fstream>
#include <functional>
#include <mutex>
#include <span>
#include <unordered_map>

/// See main.cpp for initalization
extern const struct _cl_icd_dispatch *TargetDispatch;

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

  /// @brief Allocates a new VizInstance tied to a command-buffer
  /// @param[in] CB The command-buffer to map to the created instance.
  /// @param[in] CQ The command-queue the command-buffer was created with.
  void createVizInstance(cl_command_buffer_khr CB, cl_command_queue CQ);

  /// @brief Frees heap allocated VizInstance and removes it from @a MInstances
  /// @param[in] VI Pointer to VizInstance object to free and erase.
  void destroyVizInstance(VizInstance *VI);

  /// @brief Frees the heap allocated VizInstance matching a command-buffer.
  /// @param[in] CB the command-buffer to delete the instance for.
  void destroyVizInstance(cl_command_buffer_khr CB);

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

  /// @brief Finds the VizInstance for the command-buffer, and allocates a new
  /// VizNode instance.
  /// @param[in] CB OpenCL command-buffer the node was created on
  /// @param[in] Name Entry-point name that created the node
  /// @param[in] Deps Command-buffer command sync-point dependencies
  /// @param[out] RetSyncPoint Command-buffer command returned sync-point
  void createVizNode(cl_command_buffer_khr CB, const char *Name,
                     std::span<const cl_sync_point_khr> Deps,
                     cl_sync_point_khr *RetSyncPoint) {
    std::lock_guard<std::mutex> Lock(MMutex);

    auto Itr = MCommandBufferInstanceMap.find(CB);
    assert(Itr != MCommandBufferInstanceMap.end());
    Itr->second->createVizNode(Name, Deps, RetSyncPoint);
  }

  /// @brief Finds the VizInstance for the command-buffer, and allocates a new
  /// VizNode instance representing a barrier command
  /// @param[in] CB OpenCL command-buffer the node was created on
  /// @param[in] Deps Command-buffer command sync-point dependencies
  /// @param[out] RetSyncPoint Command-buffer command returned sync-point
  void createVizBarrierNode(cl_command_buffer_khr CB,
                            std::span<const cl_sync_point_khr> Deps,
                            cl_sync_point_khr *RetSyncPoint) {
    std::lock_guard<std::mutex> Lock(MMutex);

    auto Itr = MCommandBufferInstanceMap.find(CB);
    assert(Itr != MCommandBufferInstanceMap.end());
    Itr->second->createVizBarrierNode(Deps, RetSyncPoint);
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

  /// @brief Writes out the command-buffer DAG to dot file.
  /// @param[in] CB The command-buffer to flush.
  /// @param[in] FilePath the path on disk to create the dot file.
  void flushCommandBuffer(cl_command_buffer_khr CB, const char *FilePath);

  /// @brief Query for if verbose environment variable is set
  bool verbose() const noexcept { return MVerbose; }

  /// @brief Query for if extension environment variable is set
  bool useExt() const noexcept { return MExt; }

  /*
   * Function pointers to the cl_khr_command_buffer entry-points returned
   * by the underlying driver from `clGetExtensionFunctionAddressForPlatform()`
   * so that we can call them from our equivalent shim entry-points.
   */
  clCreateCommandBufferKHR_fn MclCreateCommandBufferKHRFnPtr = nullptr;
  clCommandBarrierWithWaitListKHR_fn MclCommandBarrierWithWaitListKHRFnPtr =
      nullptr;
  clCommandCopyBufferKHR_fn MclCommandCopyBufferKHRFnPtr = nullptr;
  clCommandCopyBufferRectKHR_fn MclCommandCopyBufferRectKHRFnPtr = nullptr;
  clCommandCopyBufferToImageKHR_fn MclCommandCopyBufferToImageKHRFnPtr =
      nullptr;
  clCommandCopyImageKHR_fn MclCommandCopyImageKHRFnPtr = nullptr;
  clCommandCopyImageToBufferKHR_fn MclCommandCopyImageToBufferKHRFnPtr =
      nullptr;
  clCommandFillBufferKHR_fn MclCommandFillBufferKHRFnPtr = nullptr;
  clCommandFillImageKHR_fn MclCommandFillImageKHRFnPtr = nullptr;
  clCommandNDRangeKernelKHR_fn MclCommandNDRangeKernelKHRFnPtr = nullptr;

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

  /// Map from CL command-buffer handles, to the Viz instances holding their
  /// DAG.
  std::unordered_map<cl_command_buffer_khr, VizInstance *>
      MCommandBufferInstanceMap;

  /// Only one thread can access the context at a time.
  std::mutex MMutex;
};

/// Get the singleton instance of the context
/// @return Reference to the context instance
VizContext &getVizContext();
