// Copyright (c) 2025 Ewan Crawford
#pragma once

#include <CL/cl.h>
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
struct VizDotFile;

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
