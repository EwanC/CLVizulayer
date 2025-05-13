// Copyright (c) 2025 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clReleaseCommandQueue()";
// CHECK-NEXT: node_0[label="clEnqueueCopyBufferRect
// CHECK-NEXT: src_buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: dst_buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: src_origin = {0, 0, 0}
// CHECK-NEXT: dst_origin = {0, 0, 0}
// CHECK-NEXT: region = {2, 2, 1}
// CHECK-NEXT: src_row_pitch = 0
// CHECK-NEXT: src_slice_pitch = 0
// CHECK-NEXT: dst_row_pitch = 0
// CHECK-NEXT: dst_slice_pitch = 0"];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State;

  size_t Origin[3] = {0, 0, 0};
  size_t Region[3] = {2, 2, 1};
  size_t SlicePitch = 0;
  size_t RowPitch = 0;
  cl_int Ret = clEnqueueCopyBufferRect(
      State.OutOfOrderQueue, State.BufferA, State.BufferB, Origin, Origin,
      Region, RowPitch, SlicePitch, RowPitch, SlicePitch, 0, nullptr, nullptr);
  CHECK(Ret);

  return 0;
}
