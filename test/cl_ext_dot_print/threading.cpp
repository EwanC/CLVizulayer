// Copyright (c) 2025 Ewan Crawford

// RUN: %build -DVIZ_TEST_FILE_NAME1=\"%T/1_%basename_t.dot\" \
// RUN: -DVIZ_TEST_FILE_NAME2=\"%T/2_%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t
// RUN: FileCheck --input_file %T/1_%basename_t.dot %s
// RUN: FileCheck --input_file %T/2_%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "Viz instance teardown";
// CHECK-NEXT: [[NODE1:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE2:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE3:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE4:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE5:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE6:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE7:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE8:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE9:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE10:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE11:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE12:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE13:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE14:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE15:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: [[NODE16:node_[0-9]+]][label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: }
// CHECK-NEXT: [[NODE1]] -> [[NODE2]]
// CHECK-NEXT: [[NODE2]] -> [[NODE3]]
// CHECK-NEXT: [[NODE3]] -> [[NODE4]]
// CHECK-NEXT: [[NODE4]] -> [[NODE5]]
// CHECK-NEXT: [[NODE5]] -> [[NODE6]]
// CHECK-NEXT: [[NODE6]] -> [[NODE7]]
// CHECK-NEXT: [[NODE7]] -> [[NODE8]]
// CHECK-NEXT: [[NODE8]] -> [[NODE9]]
// CHECK-NEXT: [[NODE9]] -> [[NODE10]]
// CHECK-NEXT: [[NODE10]] -> [[NODE11]]
// CHECK-NEXT: [[NODE11]] -> [[NODE12]]
// CHECK-NEXT: [[NODE12]] -> [[NODE13]]
// CHECK-NEXT: [[NODE13]] -> [[NODE14]]
// CHECK-NEXT: [[NODE14]] -> [[NODE15]]
// CHECK-NEXT: [[NODE15]] -> [[NODE16]]
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"
#include <thread>
#include <vector>

int main() {
  CLState State(true);

  const unsigned NumThreads = 4;
  std::vector<std::thread> Threads;
  Threads.reserve(NumThreads);

  cl_int Ret = CL_SUCCESS;
  cl_dot_graph_ext Dot1 = State.clCreateDotGraphEXT(
      1, &State.InOrderQueue, nullptr, VIZ_TEST_FILE_NAME1, &Ret);
  CHECK(Ret);

  cl_dot_graph_ext Dot2 = State.clCreateDotGraphEXT(
      1, &State.InOrderQueue, nullptr, VIZ_TEST_FILE_NAME2, &Ret);
  CHECK(Ret);

  const unsigned CommandsPerThread = 4;
  auto SubmitCommands = [&]() {
    for (unsigned i = 0; i < CommandsPerThread; ++i) {
      Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                                   &State.GlobalSize, nullptr, 0, nullptr,
                                   nullptr);
      CHECK(Ret);
    }
  };

  for (unsigned i = 0; i < NumThreads; ++i) {
    Threads.emplace_back(SubmitCommands);
  }

  for (unsigned i = 0; i < NumThreads; ++i) {
    Threads[i].join();
  }

  Ret = State.clReleaseDotGraphEXT(Dot1);
  CHECK(Ret);

  Ret = State.clReleaseDotGraphEXT(Dot2);
  CHECK(Ret);

  return 0;
}
