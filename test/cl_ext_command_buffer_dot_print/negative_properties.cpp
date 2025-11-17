// Copyright (c) 2025 Ewan Crawford

// RUN: %build -DVIZ_TEST_FILE_NAME=\"%T/%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t

#include "../common.h"

int main() {
  CLState State(true);

  cl_int Ret = CL_SUCCESS;
  cl_command_buffer_khr command_buffer =
      State.clCreateCommandBufferKHR(1, &State.OutOfOrderQueue, nullptr, &Ret);
  CHECK(Ret);
  CHECK_NOT_NULL(command_buffer);

  // Invalid property name
  {
    cl_command_buffer_dot_print_flags_ext Flags =
        CL_COMMAND_BUFFER_DOT_PRINT_COLOR_EXT;
    cl_command_buffer_dot_print_properties_ext Props[] = {0x4, Flags, 0};
    cl_int Ret = State.clDotPrintCommandBufferEXT(command_buffer, Props,
                                                  VIZ_TEST_FILE_NAME);
    CHECK_ERR(Ret, CL_INVALID_VALUE);
  }

  // Invalid flags
  {
    cl_command_buffer_dot_print_flags_ext Flags = -1;
    cl_command_buffer_dot_print_properties_ext Props[] = {
        CL_COMMAND_BUFFER_DOT_PRINT_FLAGS_EXT, Flags, 0};
    cl_int Ret = State.clDotPrintCommandBufferEXT(command_buffer, Props,
                                                  VIZ_TEST_FILE_NAME);
    CHECK_ERR(Ret, CL_INVALID_VALUE);
  }

  // Invalid duplicate property name
  {
    cl_command_buffer_dot_print_flags_ext Flags =
        CL_COMMAND_BUFFER_DOT_PRINT_COLOR_EXT;
    cl_command_buffer_dot_print_properties_ext Props[] = {
        CL_COMMAND_BUFFER_DOT_PRINT_FLAGS_EXT, Flags,
        CL_COMMAND_BUFFER_DOT_PRINT_FLAGS_EXT, 0, 0};

    cl_int Ret = State.clDotPrintCommandBufferEXT(command_buffer, Props,
                                                  VIZ_TEST_FILE_NAME);
    CHECK_ERR(Ret, CL_INVALID_VALUE);
  }

  CHECK(State.clReleaseCommandBufferKHR(command_buffer));
  return 0;
}
