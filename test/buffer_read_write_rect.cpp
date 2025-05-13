// Copyright (c) 2025 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clEnqueueReadBufferRect()";
// CHECK-NEXT: node_0[label="clEnqueueReadBufferRect"];
// CHECK-NEXT: }
// CHECK-NEXT: subgraph cluster_1 {
// CHECK-NEXT: label = "clEnqueueWriteBufferRect()";
// CHECK-NEXT: node_1[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_2[label="clEnqueueWriteBufferRect"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: node_1 -> node_2
// CHECK-NEXT: node_0 -> node_1 [label="blocking host wait"
// CHECK-SAME: ltail=cluster_0 lhead=cluster_1];
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"
#include <array>

int main() {
  CLState State;

  size_t Origin[3] = {0, 0, 0};
  size_t Region[3] = {2, 2, 1};
  size_t SlicePitch = 0;
  size_t RowPitch = 0;

  std::array<cl_uchar, State.AllocSize> HostData;
  void *Ptr = (void *)HostData.data();

  cl_int Ret = clEnqueueReadBufferRect(
      State.InOrderQueue, State.BufferA, CL_TRUE, Origin, Origin, Region,
      RowPitch, SlicePitch, RowPitch, SlicePitch, Ptr, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueWriteBufferRect(
      State.InOrderQueue, State.BufferA, CL_TRUE, Origin, Origin, Region,
      RowPitch, SlicePitch, RowPitch, SlicePitch, Ptr, 0, nullptr, nullptr);
  CHECK(Ret);

  return 0;
}
