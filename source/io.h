// Copyright (c) 2025 Ewan Crawford

#pragma once

#include <fstream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

/// Forward declarations
struct VizNode;
struct VizQueue;

/// @brief Comparison function for std::set
/// @detail nodes are printed in ascending order in the dot file.
/// @param[in] LHS Left hand side of comparison
/// @param[in] RHS Right hand side of comparison
/// @return True if @a LHS is less than @a RHS
bool vizNodeCmp(VizNode *LHS, VizNode *RHS);

/// @brief RAII wrapper for output DOT file
struct VizDotFile {
  /// @brief Constructor, opens @a MDotFile
  /// @param Color Whether to print nodes with color.
  /// @param FilePath User specified file path, or nullptr if the layer decides.
  VizDotFile(bool Color, const char *FilePath);

  /// @brief Destructor, closes @a MDotFile
  ~VizDotFile();

  /// @brief Writes a node definition to dot file if it doesn't already exist
  /// @param[in] Node node to be defined
  /// @param[in] Verbose use verbose printing of node
  void writeNode(VizNode *Node, bool Verbose);

  /// @brief Writes the edge dependencies of a node to dot file
  /// @param[in] Node The node who's edges will be written
  void writeEdges(const VizNode *Node);

  /// @brief Writes a subgraph cluster to the dot file.
  /// See https://graphviz.org/Gallery/directed/cluster.html
  /// @param[in] Nodes List of nodes to define in the cluster
  /// @param[in] Label User visible title for the cluster.
  /// @param[in] Verbose Use verbose printing
  void writeSubgraph(const std::set<VizNode *, decltype(&vizNodeCmp)> &Nodes,
                     const char *Label, bool Verbose);

  /// Whether to use fill color for nodes
  bool MColor;

  /// File used for @a MDotFile
  std::string MFileName;

  /// Output stream instance for dot file being written
  std::ofstream MDotFile;

  /// Map from VizQueue objects to color
  std::unordered_map<VizQueue *, const char *> MQueueColors;

private:
  /// @brief Creates a timestamp for the current date and time
  /// @return String in format 'YYYY-MM-DD_HH_MM_SS'
  static std::string getTimestamp();

  /// @brief Generates the path where the dot file should be created
  /// @return String containing absolute path to file
  static std::string getDotFilename();

  /// @brief Finds a new color to associate with a queue.
  /// @brief VQ Queue to be added to @a MQueueColors
  /// @return New color associated with @a VQ
  const char *newColor(VizQueue *VQ);

  /// @brief Unique ID for each cluster, incrementing from zero
  unsigned MClusterID;

  /// @brief First node in previous cluster. Used to plot composite edges
  /// for blocking waits
  VizNode *MPrevClusterNode;
};
