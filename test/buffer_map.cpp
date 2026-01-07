// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clEnqueueMapBuffer()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_1[label="clEnqueueMapBuffer"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: subgraph cluster_1 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_2[label="clEnqueueUnmapMemObject"];
// CHECK-NEXT: }
// CHECK-NEXT: node_1 -> node_2
// CHECK-NEXT: node_0 -> node_2 [label="blocking host wait"
// CHECK-SAME: ltail=cluster_0 lhead=cluster_1];
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"

int main() {
  CLState State;

  cl_int Ret =
      clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                             &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  cl_map_flags Flags = CL_MAP_WRITE;
  void *Ptr =
      clEnqueueMapBuffer(State.InOrderQueue, State.BufferA, CL_TRUE, Flags, 0,
                         State.AllocSize, 0, nullptr, nullptr, &Ret);
  CHECK(Ret);

  cl_int *CastedPtr = (cl_int *)Ptr;
  CastedPtr[0] = 42;

  Ret = clEnqueueUnmapMemObject(State.InOrderQueue, State.BufferA, Ptr, 0,
                                nullptr, nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.InOrderQueue));
  return 0;
}
