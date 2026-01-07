// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_0[label="clEnqueueReadBuffer
// CHECK-NEXT: blocking = False
// CHECK-NEXT: buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: offset = 0
// CHECK-NEXT: size = 8
// CHECK-NEXT: ptr = 0x{{[0-9a-fA-F]+}}"]
// CHECK-NEXT: node_1[label="clEnqueueWriteBuffer
// CHECK-NEXT: blocking = False
// CHECK-NEXT: buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: offset = 4
// CHECK-NEXT: size = 4
// CHECK-NEXT: ptr = 0x{{[0-9a-fA-F]+}}"]
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State;

  cl_int Foo[2] = {42, 0xA};
  cl_int Ret = clEnqueueReadBuffer(State.InOrderQueue, State.BufferA, false, 0,
                                   sizeof(Foo), &Foo[1], 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueWriteBuffer(State.InOrderQueue, State.BufferA, false,
                             sizeof(cl_int), sizeof(cl_int), Foo, 0, nullptr,
                             nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.InOrderQueue));

  return 0;
}
