// Copyright (c) 2025-2026 Ewan Crawford

// REQUIRES: svm-support

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
// CHECK-NEXT: node_2[label="clEnqueueSVMMemFill"];
// CHECK-NEXT: node_3[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_4[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: }
// CHECK-NEXT: node_1 -> node_2
// CHECK-NEXT: node_2 -> node_3
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"

int main() {
  CLState State;

  cl_int Ret =
      clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                             &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  cl_event Events[2] = {nullptr, nullptr};
  Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr,
                               &Events[0]);
  CHECK(Ret);

  cl_uint Pattern = 42;
  Ret = clEnqueueSVMMemFill(State.OutOfOrderQueue, State.SVMA, &Pattern,
                            sizeof(Pattern), State.AllocSize, 1, &Events[0],
                            &Events[1]);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 1, &Events[1],
                               nullptr);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  CHECK(clReleaseEvent(Events[0]));
  CHECK(clReleaseEvent(Events[1]));
  return 0;
}
