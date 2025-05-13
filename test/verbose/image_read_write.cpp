// Copyright (c) 2025 Ewan Crawford

// REQUIRES: image-support

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_0[label="clEnqueueReadImage
// CHECK-NEXT: image = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: blocking = False
// CHECK-NEXT: origin = {0, 0, 0}
// CHECK-NEXT: region = {2, 2, 1}
// CHECK-NEXT: row_pitch = 128
// CHECK-NEXT: slice_pitch = 0\n
// CHECK-NEXT: ptr = 0x{{[0-9a-fA-F]+}}"]
// CHECK-NEXT: node_1[label="clEnqueueWriteImage
// CHECK-NEXT: image = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: blocking = False
// CHECK-NEXT: origin = {0, 0, 0}
// CHECK-NEXT: region = {2, 2, 1}
// CHECK-NEXT: row_pitch = 128
// CHECK-NEXT: slice_pitch = 0\n
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

  constexpr size_t PixelBytes = 4;
  size_t RowPitch = State.AllocSize * PixelBytes;
  size_t SlicePitch = 0;

  constexpr size_t ImageBytes = State.AllocSize * State.AllocSize * PixelBytes;
  std::array<cl_uchar, ImageBytes> HostData;
  void *Ptr = (void *)HostData.data();
  cl_int Ret = clEnqueueReadImage(State.InOrderQueue, State.ImageA, CL_FALSE,
                                  Origin, Region, RowPitch, SlicePitch, Ptr, 0,
                                  nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueWriteImage(State.InOrderQueue, State.ImageA, CL_FALSE, Origin,
                            Region, RowPitch, SlicePitch, Ptr, 0, nullptr,
                            nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.InOrderQueue));

  return 0;
}
