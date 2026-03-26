// Copyright (c) 2025-2026 Ewan Crawford

// REQUIRES: intel-usm

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clEnqueueMemcpyINTEL()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_1[label="clEnqueueMemcpyINTEL"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: subgraph cluster_1 {
// CHECK-NEXT: label = "clReleaseCommandQueue()";
// CHECK-NEXT: node_2[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: }
// CHECK-NEXT: node_1 -> node_2
// CHECK-NEXT: node_0 -> node_2 [label="blocking host wait"
// CHECK-SAME: ltail=cluster_0 lhead=cluster_1];
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"

int main() {
  CLState State;

  cl_int Ret =
      clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                             &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clEnqueueMemcpyINTEL(State.InOrderQueue, CL_TRUE, State.USMB,
                                   State.USMA, State.AllocSize, 0, nullptr,
                                   nullptr);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  return 0;
}
