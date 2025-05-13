// Copyright (c) 2025 Ewan Crawford

// REQUIRES: image-support

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clReleaseCommandQueue()";
// CHECK-NEXT: node_0[label="clEnqueueCopyBufferToImage
// CHECK-NEXT: src_buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: dst_image = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: offset = 0
// CHECK-NEXT: dst_origin = {0, 0, 0}
// CHECK-NEXT: region = {1, 1, 1}"]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State;

  size_t Origin[3] = {0, 0, 0};
  size_t Region[3] = {1, 1, 1};
  cl_int Ret = clEnqueueCopyBufferToImage(State.OutOfOrderQueue, State.BufferA,
                                          State.ImageA, 0, Origin, Region, 0,
                                          nullptr, nullptr);
  CHECK(Ret);
  return 0;
}
