// Copyright (c) 2025 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clReleaseCommandQueue()";
// CHECK-NEXT: node_0[label="clEnqueueCopyBuffer
// CHECK-NEXT: src_buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: dst_buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: src_offset = 1
// CHECK-NEXT: dst_offset = 1
// CHECK-NEXT: size = 31"];

// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State;

  cl_int Ret =
      clEnqueueCopyBuffer(State.OutOfOrderQueue, State.BufferA, State.BufferB,
                          1, 1, State.AllocSize - 1, 0, nullptr, nullptr);
  CHECK(Ret);

  return 0;
}
