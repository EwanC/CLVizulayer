// Copyright (c) 2025 Ewan Crawford
#pragma once

#include <CL/cl.h>
#include <vector>

/// Forward declarations
struct VizNode;

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
