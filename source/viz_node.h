// Copyright (c) 2025 Ewan Crawford
#pragma once

#include <fstream>
#include <functional>
#include <vector>

/// Forward declarations
struct VizQueue;

/// @brief Represents a graph node
struct VizNode {
  /// @brief Constructor for queue enqueued node
  VizNode(VizQueue *VQ, std::vector<VizNode *> &&Deps, const char *Name,
          std::function<void(std::ofstream &)> VerbosePrint)
      : MQueue(VQ), MPreds(Deps), MName(Name), MVerbosePrint(VerbosePrint),
        MDefined(false) {
    static int ID = 0;
    MID = ID++;
  }

  /// @brief Constructor for command-buffer node
  VizNode(std::vector<VizNode *> &&Deps, const char *Name,
          std::function<void(std::ofstream &)> VerbosePrint)
      : MQueue(nullptr), MPreds(Deps), MName(Name), MVerbosePrint(VerbosePrint),
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
