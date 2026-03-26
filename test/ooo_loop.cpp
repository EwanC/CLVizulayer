// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clFinish()";
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
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"

int main() {
  CLState State;

  for (unsigned int i = 0; i < 10; i++) {
    cl_int Ret =
        clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
    CHECK(Ret);
  }

  cl_int Ret = clFinish(State.OutOfOrderQueue);
  CHECK(Ret);

  return 0;
}
