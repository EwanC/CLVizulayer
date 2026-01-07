// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -DVIZ_TEST_FILE_NAME=\"%T/%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=filled]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "cl_command_buffer_khr";
// CHECK-NEXT: node_0[label="clCommandNDRangeKernelKHR
// CHECK-NEXT: cl_kernel = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: name = no_op", color="aliceblue"];
// CHECK-NEXT: node_1[label="clCommandNDRangeKernelKHR
// CHECK-NEXT: cl_kernel = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: name = no_op", color="aliceblue"];
// CHECK-NEXT: node_2[label="clCommandNDRangeKernelKHR
// CHECK-NEXT: cl_kernel = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: name = no_op", color="aliceblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: node_1 -> node_2
// CHECK-EMPTY:
// CHECK-NEXT: }

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

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 0, nullptr, nullptr, nullptr);

  Ret = State.clCommandNDRangeKernelKHR(
      CommandBuffer, nullptr, nullptr, State.Kernel, 1, nullptr,
      &State.GlobalSize, nullptr, 0, nullptr, nullptr, nullptr);

  cl_command_buffer_dot_print_flags_ext Flags =
      CL_COMMAND_BUFFER_DOT_PRINT_COLOR_EXT |
      CL_COMMAND_BUFFER_DOT_PRINT_VERBOSE_EXT;
  cl_command_buffer_dot_print_properties_ext Props[3] = {
      CL_COMMAND_BUFFER_DOT_PRINT_FLAGS_EXT, Flags, 0};

  Ret = State.clDotPrintCommandBufferEXT(CommandBuffer, Props,
                                         VIZ_TEST_FILE_NAME);
  CHECK(Ret);

  CHECK(State.clReleaseCommandBufferKHR(CommandBuffer));
  return 0;
}
