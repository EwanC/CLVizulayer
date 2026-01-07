// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clEnqueueReadBuffer()";
// CHECK-NEXT: node_0[label="clEnqueueReadBuffer"];
// CHECK-NEXT: }
// CHECK-NEXT: subgraph cluster_1 {
// CHECK-NEXT: label = "clEnqueueWriteBuffer()";
// CHECK-NEXT: node_1[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_2[label="clEnqueueWriteBuffer"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: node_1 -> node_2
// CHECK-NEXT: node_0 -> node_1 [label="blocking host wait"
// CHECK-SAME: ltail=cluster_0 lhead=cluster_1];
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"

int main() {
  CLState State;

  cl_int Foo = 42;
  cl_int Ret = clEnqueueReadBuffer(State.InOrderQueue, State.BufferA, CL_TRUE,
                                   0, sizeof(Foo), &Foo, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueWriteBuffer(State.InOrderQueue, State.BufferA, CL_TRUE, 0,
                             sizeof(Foo), &Foo, 0, nullptr, nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.InOrderQueue));
  return 0;
}
