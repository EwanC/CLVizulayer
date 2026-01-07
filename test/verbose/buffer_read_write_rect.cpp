// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_0[label="clEnqueueReadBufferRect
// CHECK-NEXT: blocking = False
// CHECK-NEXT: buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: buffer_origin = {0, 0, 0}
// CHECK-NEXT: host_origin = {0, 0, 0}
// CHECK-NEXT: region = {2, 2, 1}
// CHECK-NEXT: buffer_row_pitch = 0
// CHECK-NEXT: buffer_slice_pitch = 0
// CHECK-NEXT: host_row_pitch = 0
// CHECK-NEXT: host_slice_pitch = 0
// CHECK-NEXT: ptr = 0x{{[0-9a-fA-F]+}}"]
// CHECK-NEXT: node_1[label="clEnqueueWriteBufferRect
// CHECK-NEXT: blocking = False
// CHECK-NEXT: buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: buffer_origin = {0, 0, 0}
// CHECK-NEXT: host_origin = {0, 0, 0}
// CHECK-NEXT: region = {2, 2, 1}
// CHECK-NEXT: buffer_row_pitch = 0
// CHECK-NEXT: buffer_slice_pitch = 0
// CHECK-NEXT: host_row_pitch = 0
// CHECK-NEXT: host_slice_pitch = 0
// CHECK-NEXT: ptr = 0x{{[0-9a-fA-F]+}}"]
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"
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
      State.InOrderQueue, State.BufferA, CL_FALSE, Origin, Origin, Region,
      RowPitch, SlicePitch, RowPitch, SlicePitch, Ptr, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueWriteBufferRect(
      State.InOrderQueue, State.BufferA, CL_FALSE, Origin, Origin, Region,
      RowPitch, SlicePitch, RowPitch, SlicePitch, Ptr, 0, nullptr, nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.InOrderQueue));

  return 0;
}
