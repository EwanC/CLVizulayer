// Copyright (c) 2025 Ewan Crawford

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
// CHECK-NEXT: node_3[label="clEnqueueBarrierWithWaitList"];
// CHECK-NEXT: node_4[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_5[label="clEnqueueBarrierWithWaitList"];
// CHECK-NEXT: }
// CHECK-NEXT: node_1 -> node_3
// CHECK-NEXT: node_3 -> node_4
// CHECK-NEXT: node_3 -> node_5
// CHECK-NEXT: node_0 -> node_5
// CHECK-NEXT: node_2 -> node_5
// CHECK-NEXT: node_4 -> node_5
// CHECK-NEXT: subgraph cluster_1 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_6[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_7[label="clEnqueueBarrierWithWaitList"];
// CHECK-NEXT: }
// CHECK-NEXT: node_5 -> node_7
// CHECK-NEXT: node_6 -> node_7
// CHECK-NEXT: node_0 -> node_6 [label="blocking host wait"
// CHECK-SAME: ltail=cluster_0 lhead=cluster_1];
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"

int main() {
  CLState State;

  cl_int Ret =
      clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                             &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  cl_event KernelEvent;
  Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr,
                               &KernelEvent);

  Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);

  Ret = clEnqueueBarrierWithWaitList(State.OutOfOrderQueue, 1, &KernelEvent,
                                     nullptr);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);

  cl_event BarrierEvent;
  Ret = clEnqueueBarrierWithWaitList(State.OutOfOrderQueue, 0, nullptr,
                                     &BarrierEvent);
  CHECK(Ret);

  Ret = clFinish(State.OutOfOrderQueue);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueBarrierWithWaitList(State.InOrderQueue, 1, &BarrierEvent,
                                     nullptr);
  CHECK(Ret);

  Ret = clFinish(State.InOrderQueue);
  CHECK(Ret);

  CHECK(clReleaseEvent(KernelEvent));
  CHECK(clReleaseEvent(BarrierEvent));

  return 0;
}
