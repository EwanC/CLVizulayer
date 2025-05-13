// Copyright (c) 2025 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clReleaseCommandQueue()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_1[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_2[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_3[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_4[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_5[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_6[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_7[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_8[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_9[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_10[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_11[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_12[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_13[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_14[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_15[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: node_1 -> node_2
// CHECK-NEXT: node_2 -> node_3
// CHECK-NEXT: node_3 -> node_4
// CHECK-NEXT: node_4 -> node_5
// CHECK-NEXT: node_5 -> node_6
// CHECK-NEXT: node_6 -> node_7
// CHECK-NEXT: node_7 -> node_8
// CHECK-NEXT: node_8 -> node_9
// CHECK-NEXT: node_9 -> node_10
// CHECK-NEXT: node_10 -> node_11
// CHECK-NEXT: node_11 -> node_12
// CHECK-NEXT: node_12 -> node_13
// CHECK-NEXT: node_13 -> node_14
// CHECK-NEXT: node_14 -> node_15
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"
#include <thread>
#include <vector>

int main() {
  CLState State;

  const unsigned NumThreads = 4;
  std::vector<std::thread> Threads;
  Threads.reserve(NumThreads);

  const unsigned CommandsPerThread = 4;
  auto SubmitCommands = [&]() {
    cl_int Ret = CL_SUCCESS;
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

  return 0;
}
