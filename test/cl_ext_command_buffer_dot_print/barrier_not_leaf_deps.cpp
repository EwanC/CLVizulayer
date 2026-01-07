// Copyright (c) 2025-2026 Ewan Crawford

// REQUIRES: command-buffer

// RUN: %build -DVIZ_TEST_FILE_NAME=\"%T/%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "cl_command_buffer_khr";
// CHECK-NEXT: node_0[label="clCommandNDRangeKernelKHR"];
// CHECK-NEXT: node_1[label="clCommandNDRangeKernelKHR"];
// CHECK-NEXT: node_2[label="clCommandBarrierWithWaitListKHR"];
// CHECK-NEXT: node_3[label="clCommandNDRangeKernelKHR"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: node_0 -> node_2
// CHECK-NEXT: node_2 -> node_3
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State(true);

  cl_int Ret = CL_SUCCESS;
  cl_command_buffer_khr CommandBuffer =
      State.clCreateCommandBufferKHR(1, &State.OutOfOrderQueue, nullptr, &Ret);
  CHECK(Ret);
  CHECK_NOT_NULL(CommandBuffer);

  cl_sync_point_khr SyncPoint;

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 0, nullptr, &SyncPoint, nullptr);
  CHECK(Ret);

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 1, &SyncPoint, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clCommandBarrierWithWaitListKHR(CommandBuffer, nullptr, nullptr,
                                              1, &SyncPoint, nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 0, nullptr, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clDotPrintCommandBufferEXT(CommandBuffer, nullptr,
                                         VIZ_TEST_FILE_NAME);
  CHECK(Ret);

  CHECK(State.clReleaseCommandBufferKHR(CommandBuffer));

  return 0;
}
