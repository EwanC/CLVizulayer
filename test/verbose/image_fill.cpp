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
// CHECK-NEXT: node_0[label="clEnqueueFillImage
// CHECK-NEXT: cl_mem = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: origin = {1, 1, 0}
// CHECK-NEXT: region = {2, 2, 1}
// CHECK-NEXT: color ptr = 0x{{[0-9a-fA-F]+}}"];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State;

  cl_float Color[4] = {1.0f, 2.0f, 4.0f, 8.0f};
  size_t Origin[3] = {1, 1, 0};
  size_t Region[3] = {2, 2, 1};
  cl_int Ret = clEnqueueFillImage(State.OutOfOrderQueue, State.ImageA, &Color,
                                  Origin, Region, 0, nullptr, nullptr);
  CHECK(Ret);

  return 0;
}
