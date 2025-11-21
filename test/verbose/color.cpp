// Copyright (c) 2025 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_COLOR=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=filled]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel
// CHECK-NEXT: cl_kernel = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: name = no_op", color="aliceblue"];
// CHECK-NEXT: node_1[label="clEnqueueNDRangeKernel
// CHECK-NEXT: cl_kernel = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: name = no_op", color="aqua"];
// CHECK-NEXT: node_2[label="clEnqueueNDRangeKernel
// CHECK-NEXT: cl_kernel = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: name = no_op", color="aliceblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: node_1 -> node_2
// CHECK-NEXT: node_0 -> node_2
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State;

  cl_event Events[2] = {nullptr, nullptr};
  cl_int Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1,
                                      nullptr, &State.GlobalSize, nullptr, 0,
                                      nullptr, &Events[0]);

  Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 1, &Events[0],
                               &Events[1]);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 1, &Events[1],
                               nullptr);
  CHECK(Ret);

  CHECK(clFlush(State.OutOfOrderQueue));
  CHECK(clFinish(State.InOrderQueue));

  CHECK(clReleaseEvent(Events[0]));
  CHECK(clReleaseEvent(Events[1]));

  return 0;
}
