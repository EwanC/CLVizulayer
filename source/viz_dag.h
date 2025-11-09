// Copyright (c) 2025 Ewan Crawford
#pragma once

#define CL_ENABLE_BETA_EXTENSIONS
#include <CL/cl_ext.h>
#include <unordered_map>
#include <vector>

/// Forward declarations
struct VizNode;

struct VizDAG {
  VizDAG(bool IsInOrder)
      : MIsInOrder(IsInOrder), MLastNode(nullptr), MLastBarrier(nullptr) {}

  /// Whether the graph is in-order or out-of-order
  bool MIsInOrder;
  /// The last node added to graph, nullptr if @a MIsInOrder is false.
  VizNode *MLastNode;
  /// The last barrier command added to graph
  VizNode *MLastBarrier;
  /// Leaf nodes of the queue
  std::vector<VizNode *> MLeaves;
};

/// @brief Tracks scheduling information relating to a command-queue
struct VizQueue : VizDAG {
  /// @brief Constructor
  VizQueue(cl_command_queue Queue, bool IsInOrder)
      : VizDAG(IsInOrder), MQueue(Queue) {}

  /// Command-queue being tracked
  cl_command_queue MQueue;
};

/// @brief Tracks scheduling information relating to a command-queue
struct VizCB : VizDAG {
  /// @brief Constructor
  VizCB(cl_command_buffer_khr CB, bool IsInOrder)
      : VizDAG(IsInOrder), MCB(CB) {}

  /// Command-buffer being tracked
  cl_command_buffer_khr MCB;

  /// Map of sync_points to VIZ nodes
  std::unordered_map<cl_sync_point_khr, VizNode *> MSyncPointMap;
};
