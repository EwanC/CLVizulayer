// Copyright (c) 2025 Ewan Crawford

// REQUIRES: command-buffer

// RUN: %build -DVIZ_TEST_FILE_NAME1=\"%T/1_%basename_t.dot\" -DVIZ_TEST_FILE_NAME2=\"%T/2_%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t
// RUN: FileCheck --check-prefix CHECK1 --input_file %T/1_%basename_t.dot %s
// RUN: FileCheck --check-prefix CHECK2 --input_file %T/2_%basename_t.dot %s

// CHECK1: digraph CLVizulayer {
// CHECK1-NEXT: compound=true
// CHECK1-NEXT: node [style=bold]
// CHECK1-NEXT: subgraph cluster_0 {
// CHECK1-NEXT: label = "cl_command_buffer_khr";
// CHECK1-NEXT: node_0[label="clCommandNDRangeKernelKHR"];
// CHECK1-NEXT: node_1[label="clCommandNDRangeKernelKHR"];
// CHECK1-NEXT: }
// CHECK1-NEXT: node_0 -> node_1
// CHECK1-EMPTY:
// CHECK1-NEXT: }

// CHECK2: digraph CLVizulayer {
// CHECK2-NEXT: compound=true
// CHECK2-NEXT: node [style=bold]
// CHECK2-NEXT: subgraph cluster_0 {
// CHECK2-NEXT: label = "cl_command_buffer_khr";
// CHECK2-NEXT: node_0[label="clCommandNDRangeKernelKHR"];
// CHECK2-NEXT: node_1[label="clCommandNDRangeKernelKHR"];
// CHECK2-NEXT: node_2[label="clCommandNDRangeKernelKHR"];
// CHECK2-NEXT: }
// CHECK2-NEXT: node_0 -> node_1
// CHECK2-NEXT: node_1 -> node_2
// CHECK2-EMPTY:
// CHECK2-NEXT: }

#include "../common.h"

int main() {
  CLState State(true);

  cl_int Ret = CL_SUCCESS;
  cl_command_buffer_khr CommandBuffer =
      State.clCreateCommandBufferKHR(1, &State.InOrderQueue, nullptr, &Ret);
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

  Ret = State.clDotPrintCommandBufferEXT(CommandBuffer, VIZ_TEST_FILE_NAME1);
  CHECK(Ret);

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 0, nullptr, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clDotPrintCommandBufferEXT(CommandBuffer, VIZ_TEST_FILE_NAME2);
  CHECK(Ret);

  CHECK(State.clReleaseCommandBufferKHR(CommandBuffer));

  return 0;
}
