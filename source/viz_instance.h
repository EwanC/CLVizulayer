// Copyright (c) 2025-2026 Ewan Crawford
#pragma once

#define CL_ENABLE_BETA_EXTENSIONS

#include "cl_ext_command_buffer_dot_print.h"
#include "cl_ext_dot_print.h"
#include <CL/cl_ext.h>
#include <array>
#include <functional>
#include <memory>
#include <mutex>
#include <span>
#include <stack>
#include <unordered_map>
#include <vector>

/// Forward declarations
struct VizNode;
struct VizQueue;
struct VizCB;
struct VizDotFile;

/// @brief Context holding the state of the layer
struct VizInstance {
  /// @brief Constructor used when VIZ_EXT=0 for whole application tracing.
  /// @param[in] Color true if color environment variable set, false otherwise
  /// @param[in] Verbose true if verbose environment variable set, false
  /// otherwise
  /// @param[in] FilePath User defined path for the DOT file, or nullptr for
  /// layer default.
  VizInstance(bool Color, bool Verbose, const char *FilePath = nullptr);

  /// @brief Constructor for a cl_dot_graph_ext instance.
  /// @oaram[in] Flags passed via properties on handle creation.
  /// @param[in] FilePath User defined path for the DOT file, or nullptr for
  /// layer default.
  VizInstance(const cl_dot_graph_flags_ext Flags,
              const char *FilePath = nullptr);

  /// @brief Constructor for a command-buffer instance.
  /// @param[in] CB command-buffer handle
  /// @oaram[in] IsInOrder whether command-buffer is in-order or not.
  VizInstance(cl_command_buffer_khr CB, bool IsInOrder);

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

  /// @brief Allocates a new VizNode instance and stores it in @a MNodes
  /// @param[in] Name Entry-point name that created the node
  /// @param[in] VerbosePrintFunc Function for printing verbose node
  /// @param[in] Deps cl_khr_command_buffer command dependencies
  /// @param[out] RetSyncPoint Returned sync-point for command
  void createVizNode(const char *Name,
                     std::function<void(std::ofstream &)> VerbosePrintFunc,
                     std::span<const cl_sync_point_khr> Deps,
                     cl_sync_point_khr *RetSyncPoint);

  /// @brief Allocates a new VizNode instance representing a barrier command,
  /// and stores it in @a MNodes
  /// @param[in] Deps cl_khr_command_buffer command dependencies
  /// @param[out] RetSyncPoint Returned sync-point for command
  void createVizBarrierNode(std::span<const cl_sync_point_khr> Deps,
                            cl_sync_point_khr *RetSyncPoint);

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

  /// @brief Write all nodes for command-buffer to dot file at passed path
  /// @param[in] Flags Bitfield configuring printing of the dot file.
  /// @param[in] FilePath Path on disk to create the dot file
  void flushCommandBuffer(const cl_command_buffer_dot_print_flags_ext Flags,
                          const char *FilePath);

  /// @brief Query for whether the instance is using verbose printing
  /// @return True if verbose, false otherwise
  bool isVerbose() const { return MVerbose; }

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

  /// @brief Generic work before a node has been created for a command-buffer
  /// command
  /// param[in] Deps Command-buffer sync-points the node depends on
  /// param[out] DepVizNodes Viz nodes the node will depend on
  /// param[in] isBarrier Whether the node being created is a barrier
  void NodePreCreation(std::span<const cl_sync_point_khr> Deps,
                       std::vector<VizNode *> &DepVizNodes,
                       bool isBarrier = false);

  /// @brief Generic work after a node has been created
  /// @param[in] Node The node which has been created
  /// @param[in] VQ Queue the node was enqueued to
  /// @param[out] RetEvent The OpenCL event returned from node creation
  void NodePostCreation(VizNode *Node, VizQueue *VQ, cl_event *RetEvent);

  /// @brief Generic work after a command-buffer node has been created
  /// @param[in] Node The node which has been created
  /// @param[out] RetSyncPoint The command-buffer sync-point returned from node
  /// creation
  void NodePostCreation(VizNode *Node, cl_sync_point_khr *RetSyncPoint);

  /// Tracks whether to use verbose dot printing
  bool MVerbose;

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

  /// Viz wrapper around cl_command_buffer_khr
  std::unique_ptr<VizCB> MCommandBuffer;
};
