// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clWaitForEvents()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_1[label="clEnqueueTask"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"

int main() {
  CLState State;

  cl_event Events[2] = {nullptr, nullptr};
  cl_int Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1,
                                      nullptr, &State.GlobalSize, nullptr, 0,
                                      nullptr, &Events[0]);
  CHECK(Ret);

  Ret = clEnqueueTask(State.OutOfOrderQueue, State.Kernel, 1, &Events[0],
                      &Events[1]);
  CHECK(Ret);

  Ret = clWaitForEvents(2, Events);
  CHECK(Ret);

  CHECK(clReleaseEvent(Events[0]));
  CHECK(clReleaseEvent(Events[1]));

  return 0;
}
