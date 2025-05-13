// Copyright (c) 2025 Ewan Crawford

// REQUIRES: svm-support

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clReleaseCommandQueue()";
// CHECK-NEXT: node_0[label="clEnqueueSVMMemFill
// CHECK-NEXT: svm_ptr = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: pattern = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: pattern_size = 8
// CHECK-NEXT: size = 32"];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State;

  cl_int2 Pattern{42, -42};
  cl_int Ret = clEnqueueSVMMemFill(State.OutOfOrderQueue, State.SVMA, &Pattern,
                                   sizeof(Pattern), State.AllocSize, 0, nullptr,
                                   nullptr);
  CHECK(Ret);

  return 0;
}
