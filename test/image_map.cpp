// Copyright (c) 2025 Ewan Crawford

// REQUIRES: image-support

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clEnqueueMapImage()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_1[label="clEnqueueMapImage"];
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

  cl_map_flags Flags = CL_MAP_READ;
  size_t Origin[3] = {0, 0, 0};
  size_t Region[3] = {2, 2, 1};
  size_t RowPitch, SlicePitch;

  void *Ptr = clEnqueueMapImage(State.InOrderQueue, State.ImageA, CL_TRUE,
                                Flags, Origin, Region, &RowPitch, &SlicePitch,
                                0, nullptr, nullptr, &Ret);
  CHECK(Ret);

  Ret = clEnqueueUnmapMemObject(State.InOrderQueue, State.ImageA, Ptr, 0,
                                nullptr, nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.InOrderQueue));
  return 0;
}
