// Copyright (c) 2025-2026 Ewan Crawford

// REQUIRES: svm-support

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clReleaseCommandQueue()";
// CHECK-NEXT: node_0[label="clEnqueueSVMMigrateMem
// CHECK-NEXT: svm_ptr = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: size = 32
// CHECK-NEXT: svm_ptr = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: size = 32
// CHECK-NEXT: flags = 1"];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State;

  const void *SVMPtrs[2] = {State.SVMA, State.SVMB};
  const size_t Sizes[2] = {State.AllocSize, State.AllocSize};
  cl_int Ret =
      clEnqueueSVMMigrateMem(State.OutOfOrderQueue, 2, SVMPtrs, Sizes,
                             CL_MIGRATE_MEM_OBJECT_HOST, 0, nullptr, nullptr);
  CHECK(Ret);

  return 0;
}
