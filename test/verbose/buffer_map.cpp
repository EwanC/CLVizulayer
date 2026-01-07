// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel
// CHECK-NEXT: cl_kernel = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: name = no_op"];
// CHECK-NEXT: node_1[label="clEnqueueMapBuffer
// CHECK-NEXT: blocking = False
// CHECK-NEXT: buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: map_flags = 2
// CHECK-NEXT: offset = 4
// CHECK-NEXT: size = 28
// CHECK-NEXT: mapped_ptr = 0x{{[0-9a-fA-F]+}}"]
// CHECK-NEXT: node_2[label="clEnqueueUnmapMemObject
// CHECK-NEXT: memobj = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: mapped_ptr = 0x{{[0-9a-fA-F]+}}"]
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: node_1 -> node_2
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State;

  cl_event Events[2] = {nullptr, nullptr};

  cl_int Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1,
                                      nullptr, &State.GlobalSize, nullptr, 0,
                                      nullptr, &Events[0]);
  CHECK(Ret);

  cl_map_flags Flags = CL_MAP_WRITE;
  void *Ptr = clEnqueueMapBuffer(
      State.OutOfOrderQueue, State.BufferA, CL_FALSE, Flags, sizeof(cl_int),
      State.AllocSize - sizeof(cl_int), 1, Events, &Events[1], &Ret);
  CHECK(Ret);

  cl_int *CastedPtr = (cl_int *)Ptr;
  CastedPtr[0] = 42;

  Ret = clEnqueueUnmapMemObject(State.OutOfOrderQueue, State.BufferA, Ptr, 1,
                                &Events[1], nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.OutOfOrderQueue));
  CHECK(clReleaseEvent(Events[0]));
  CHECK(clReleaseEvent(Events[1]));

  return 0;
}
