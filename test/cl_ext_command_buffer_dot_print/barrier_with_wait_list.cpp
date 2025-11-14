// Copyright (c) 2025 Ewan Crawford

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
// CHECK-NEXT: node_2[label="clCommandNDRangeKernelKHR"];
// CHECK-NEXT: node_3[label="clCommandBarrierWithWaitListKHR"];
// CHECK-NEXT: node_4[label="clCommandNDRangeKernelKHR"];
// CHECK-NEXT: node_5[label="clCommandBarrierWithWaitListKHR"];
// CHECK-NEXT: node_6[label="clCommandNDRangeKernelKHR"];
// CHECK-NEXT: node_7[label="clCommandNDRangeKernelKHR"];
// CHECK-NEXT: }
// CHECK-NEXT: node_1 -> node_3
// CHECK-NEXT: node_3 -> node_4
// CHECK-NEXT: node_3 -> node_5
// CHECK-NEXT: node_0 -> node_5
// CHECK-NEXT: node_2 -> node_5
// CHECK-NEXT: node_4 -> node_5
// CHECK-NEXT: node_5 -> node_6
// CHECK-NEXT: node_5 -> node_6
// CHECK-NEXT: node_5 -> node_7
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

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 0, nullptr, nullptr, nullptr);
  CHECK(Ret);

  cl_sync_point_khr KernelSyncPoint;
  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 0, nullptr, &KernelSyncPoint, nullptr);
  CHECK(Ret);

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 0, nullptr, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clCommandBarrierWithWaitListKHR(
      CommandBuffer, nullptr, nullptr, 1, &KernelSyncPoint, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 0, nullptr, nullptr, nullptr);
  CHECK(Ret);

  cl_sync_point_khr BarrierSyncPoint;
  Ret = State.clCommandBarrierWithWaitListKHR(
      CommandBuffer, nullptr, nullptr, 0, nullptr, &BarrierSyncPoint, nullptr);
  CHECK(Ret);

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 1, &BarrierSyncPoint, nullptr, nullptr);
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
