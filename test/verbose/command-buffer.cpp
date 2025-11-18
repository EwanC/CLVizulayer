// Copyright (c) 2025 Ewan Crawford

// REQUIRES: command-buffer

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_0[label="clEnqueueCommandBufferKHR\n
// CHECK-NEXT: cl_command_buffer_khr = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

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

  Ret = State.clEnqueueCommandBufferKHR(1, &State.OutOfOrderQueue,
                                        CommandBuffer, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = clFinish(State.OutOfOrderQueue);

  CHECK(State.clReleaseCommandBufferKHR(CommandBuffer));

  return 0;
}
