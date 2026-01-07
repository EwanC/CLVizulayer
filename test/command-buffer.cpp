// Copyright (c) 2025-2026 Ewan Crawford

// REQUIRES: command-buffer

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_1[label="clEnqueueCommandBufferKHR"];
// CHECK-NEXT: node_2[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: node_1 -> node_2
// CHECK-NEXT: subgraph cluster_1 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_3[label="clEnqueueCommandBufferKHR"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_3 [label="blocking host wait" ltail=cluster_0
// lhead=cluster_1]; CHECK-EMPTY: CHECK-NEXT: }

#include "common.h"

int main() {
  CLState State;

  cl_int Ret = CL_SUCCESS;
  cl_command_buffer_khr CommandBuffer =
      State.clCreateCommandBufferKHR(1, &State.OutOfOrderQueue, nullptr, &Ret);
  CHECK(Ret);
  CHECK_NOT_NULL(CommandBuffer);

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 0, nullptr, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 0, nullptr, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 0, nullptr, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clFinalizeCommandBufferKHR(CommandBuffer);
  CHECK(Ret);

  cl_event Events[2] = {nullptr, nullptr};
  Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr,
                               &Events[0]);
  CHECK(Ret);

  Ret = State.clEnqueueCommandBufferKHR(0, nullptr, CommandBuffer, 1,
                                        &Events[0], &Events[1]);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 1, &Events[1],
                               nullptr);
  CHECK(Ret);

  Ret = clFinish(State.OutOfOrderQueue);
  CHECK(Ret);

  Ret = State.clEnqueueCommandBufferKHR(1, &State.OutOfOrderQueue,
                                        CommandBuffer, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = clFinish(State.OutOfOrderQueue);

  CHECK(State.clReleaseCommandBufferKHR(CommandBuffer));
  CHECK(clReleaseEvent(Events[0]));
  CHECK(clReleaseEvent(Events[1]));

  return 0;
}
