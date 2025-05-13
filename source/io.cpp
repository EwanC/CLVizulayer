// Copyright (c) 2025 Ewan Crawford

#include "io.h"
#include "logger.h"
#include "viz.h"
#include <ctime>
#include <filesystem>
#include <sstream>

bool vizNodeCmp(VizNode *LHS, VizNode *RHS) { return LHS->MID < RHS->MID; }

std::string VizDotFile::getTimestamp() {
  const std::time_t Time = std::time(0);
  const std::tm *Now = std::localtime(&Time);
  std::ostringstream Stream;
  Stream << (Now->tm_year + 1900) << '-' << (Now->tm_mon + 1) << '-'
         << (Now->tm_mday) << '_' << (Now->tm_hour) << ':' << (Now->tm_min)
         << ':' << (Now->tm_sec);
  return Stream.str();
}

std::string VizDotFile::getDotFilename() {
  // User provided path
  if (const char *Path = std::getenv("VIZ_DOT_FILE")) {
    // If this file already exists if will be overwritten by the
    // ofstream
    return std::string(Path);
  }

  // Path to current directory
  std::filesystem::path CWD = std::filesystem::current_path();

  // Name of the file
  std::string FileName("clviz_");
  FileName.append(getTimestamp());
  FileName.append(".dot");

  std::filesystem::path FilePath = CWD / FileName;
  if (std::filesystem::exists(FilePath)) {
    // The smallest granularity of timestamp is 1 second, this is large enough
    // that it's conceivable that multiple processes could create a VizDotFile
    // instance in the same second. Suffix filenames with a unique ID in this
    // case.
    static unsigned UID = 1;
    std::string Suffix("_");
    Suffix.append(std::to_string(UID++));
    FileName.insert(FileName.length() - 4, Suffix);
    FilePath = CWD / FileName;
  }

  return FilePath.string();
}

VizDotFile::VizDotFile(bool Color, const char *FilePath)
    : MColor(Color), MClusterID(0), MPrevClusterNode(nullptr) {
  MFileName = FilePath ? FilePath : getDotFilename();
  MDotFile.open(MFileName);
  MDotFile << "digraph CLVizulayer {\n";
  MDotFile << "compound=true\n";
  if (MColor) {
    MDotFile << "node [style=filled]\n";
  } else {
    MDotFile << "node [style=bold]\n";
  }

  VIZ_LOG("Opened dot file %s", MFileName.c_str());
}

VizDotFile::~VizDotFile() {
  MDotFile << "\n}";
  MDotFile.close();
  VIZ_LOG("Closed dot file %s", MFileName.c_str());
}

const char *VizDotFile::newColor(VizQueue *VQ) {
  static const char *Colors[] = {"aliceblue",
                                 "aqua",
                                 "aquamarine",
                                 "azure",
                                 "beige",
                                 "bisque",
                                 "blue",
                                 "blueviolet",
                                 "brown",
                                 "burlywood",
                                 "cadetblue",
                                 "chartreuse",
                                 "chocolate",
                                 "coral",
                                 "cornflowerblue",
                                 "cornsilk",
                                 "crimson",
                                 "cyan",
                                 "cyan",
                                 "darkgoldenrod",
                                 "darkgray",
                                 "darkgreen",
                                 "darkkhaki",
                                 "darkmagenta",
                                 "darkolivegreen",
                                 "darkorange",
                                 "darkorchid",
                                 "darkred",
                                 "darksalmon",
                                 "darkseagreen",
                                 "darkslateblue",
                                 "darkslategray",
                                 "darkturquoise",
                                 "darkviolet",
                                 "deeppink",
                                 "deepskyblue",
                                 "dimgray",
                                 "dodgerblue",
                                 "firebrick",
                                 "floralwhite",
                                 "forestgreen",
                                 "fuchsia",
                                 "gainsboro",
                                 "gold",
                                 "green",
                                 "greenyellow",
                                 "greenyellow",
                                 "indianred",
                                 "indigo",
                                 "khaki",
                                 "lavender",
                                 "lawngreen",
                                 "lemonchiffon",
                                 "lightblue",
                                 "lightcoral",
                                 "lightgoldenrod",
                                 "lightgreen",
                                 "lightpink",
                                 "lightsalmon",
                                 "lightseagreen",
                                 "lightsteelblue",
                                 "lime",
                                 "limegreen",
                                 "magenta",
                                 "maroon",
                                 "mediumaquamarine",
                                 "mediumorchid",
                                 "mediumpurple",
                                 "mediumseagreen",
                                 "mediumvioletred",
                                 "moccasin",
                                 "olive",
                                 "orange",
                                 "orangered",
                                 "orchid",
                                 "palegreen",
                                 "paleturquoise",
                                 "palevioletred",
                                 "peachpuff",
                                 "peru",
                                 "pink",
                                 "plum",
                                 "powderblue",
                                 "purple",
                                 "red",
                                 "rosybrown",
                                 "salmon",
                                 "sandybrown",
                                 "seagreen",
                                 "sienna",
                                 "silver",
                                 "skyblue",
                                 "slateblue",
                                 "slategrey",
                                 "springgreen",
                                 "steelblue",
                                 "tan",
                                 "teal",
                                 "thistle",
                                 "tomato",
                                 "turquoise",
                                 "violet",
                                 "violetred",
                                 "wheat",
                                 "yellow",
                                 "yellowgreen"};
  constexpr size_t NumColors = sizeof(Colors) / sizeof(char *);
  static unsigned ColorIndex = 0;
  const char *QueueColor = Colors[ColorIndex++];

  if (ColorIndex > (NumColors - 1)) {
    VIZ_ERR("Number of queues exceeded pool of %zu colors\n", NumColors);
  }

  VIZ_LOG("Queue %p Color %s\n", VQ, QueueColor);

  MQueueColors.insert({VQ, QueueColor});
  return QueueColor;
}

void VizDotFile::writeNode(VizNode *Node) {
  // Don't define again if already define
  if (Node->MDefined) {
    return;
  }

  MDotFile << "node_" << Node->MID;

  MDotFile << "[label=\"";
  MDotFile << Node->MName;
  if (Node->MVerbosePrint) {
    Node->MVerbosePrint(this);
  }
  MDotFile << "\"";

  if (MColor) {
    auto Color = MQueueColors.find(Node->MQueue);
    const char *ColorStr =
        Color != MQueueColors.end() ? Color->second : newColor(Node->MQueue);
    MDotFile << ", color=\"" << ColorStr << "\"";
  }

  MDotFile << "];\n";

  Node->MDefined = true;
}

void VizDotFile::writeEdges(const VizNode *Node) {
  for (VizNode *Pred : Node->MPreds) {
    MDotFile << "node_" << Pred->MID;
    MDotFile << " -> ";
    MDotFile << "node_" << Node->MID << "\n";
  }
}

void VizDotFile::writeSubgraph(
    const std::set<VizNode *, decltype(&vizNodeCmp)> &Nodes,
    const char *Label) {
  if (Nodes.empty()) {
    return;
  }

  MDotFile << "subgraph ";
  MDotFile << "cluster_" + std::to_string(MClusterID);
  MDotFile << " {\n";
  MDotFile << "label = \"";
  MDotFile << Label;
  MDotFile << "\";\n";

  // Define nodes inside subgraph cluster
  for (auto N : Nodes) {
    writeNode(N);
  }
  MDotFile << "}\n";

  // Edges need printed after subgraph to render nodes correctly in cluster
  for (auto N : Nodes) {
    writeEdges(N);
  }

  // Write composite edge between the two clusters to represent serialization
  // from blocking host waits
  if (MPrevClusterNode) {
    MDotFile << "node_" << MPrevClusterNode->MID;
    MDotFile << " -> ";
    MDotFile << "node_" << (*Nodes.begin())->MID;

    MDotFile << " [label=\"blocking host wait\"";
    MDotFile << " ltail=cluster_" << std::to_string(MClusterID - 1);
    MDotFile << " lhead=cluster_" << std::to_string(MClusterID);
    MDotFile << "];\n";
  }
  MPrevClusterNode = *Nodes.begin();
  MClusterID++;
}
