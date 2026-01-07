// Copyright (c) 2025-2026 Ewan Crawford

// REQUIRES: command-buffer, image-support

// RUN: %build -DVIZ_TEST_FILE_NAME=\"%T/%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "cl_command_buffer_khr";
// CHECK-NEXT: node_0[label="clCommandFillImageKHR
// CHECK-NEXT: cl_mem = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: origin = {1, 1, 0}
// CHECK-NEXT: region = {2, 2, 1}
// CHECK-NEXT: color ptr = 0x{{[0-9a-fA-F]+}}"];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State(true);

  cl_int Ret = CL_SUCCESS;
  cl_command_buffer_khr command_buffer =
      State.clCreateCommandBufferKHR(1, &State.OutOfOrderQueue, nullptr, &Ret);
  CHECK(Ret);
  CHECK_NOT_NULL(command_buffer);

  cl_float Color[4] = {1.0f, 2.0f, 4.0f, 8.0f};
  size_t Origin[3] = {1, 1, 0};
  size_t Region[3] = {2, 2, 1};
  Ret = State.clCommandFillImageKHR(command_buffer, nullptr, nullptr,
                                    State.ImageA, &Color, Origin, Region, 0,
                                    nullptr, nullptr, nullptr);

  cl_command_buffer_dot_print_flags_ext Flags =
      CL_COMMAND_BUFFER_DOT_PRINT_VERBOSE_EXT;
  cl_command_buffer_dot_print_properties_ext Props[3] = {
      CL_COMMAND_BUFFER_DOT_PRINT_FLAGS_EXT, Flags, 0};
  Ret = State.clDotPrintCommandBufferEXT(command_buffer, Props,
                                         VIZ_TEST_FILE_NAME);
  CHECK(Ret);

  CHECK(State.clReleaseCommandBufferKHR(command_buffer));

  return 0;
}
