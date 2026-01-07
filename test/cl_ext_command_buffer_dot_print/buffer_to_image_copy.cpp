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
// CHECK-NEXT: node_0[label="clCommandCopyBufferToImageKHR
// CHECK-NEXT: src_buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: dst_image = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: offset = 0
// CHECK-NEXT: dst_origin = {0, 0, 0}
// CHECK-NEXT: region = {1, 1, 1}"]
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

  size_t Origin[3] = {0, 0, 0};
  size_t Region[3] = {1, 1, 1};
  Ret = State.clCommandCopyBufferToImageKHR(
      command_buffer, nullptr, nullptr, State.BufferA, State.ImageA, 0, Origin,
      Region, 0, nullptr, nullptr, nullptr);

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
