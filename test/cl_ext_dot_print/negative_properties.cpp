// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -DVIZ_TEST_FILE_NAME=\"%T/%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t

#include "../common.h"

int main() {
  CLState State(true);

  // Invalid property name
  {
    cl_dot_graph_flags_ext Flags = CL_DOT_GRAPH_COLOR_EXT;
    cl_dot_graph_properties_ext Props[] = {0x4, Flags, 0};
    cl_int Ret = CL_SUCCESS;
    cl_dot_graph_ext Dot = State.clCreateDotGraphEXT(
        1, &State.InOrderQueue, Props, VIZ_TEST_FILE_NAME, &Ret);
    CHECK_ERR(Ret, CL_INVALID_VALUE);
    if (Dot != nullptr) {
      throw std::runtime_error("NULL handle not returned on error");
    }
  }

  // Invalid flags
  {
    cl_dot_graph_flags_ext Flags = -1;
    cl_dot_graph_properties_ext Props[] = {CL_DOT_GRAPH_FLAGS_EXT, Flags, 0};
    cl_int Ret = CL_SUCCESS;
    cl_dot_graph_ext Dot = State.clCreateDotGraphEXT(
        1, &State.InOrderQueue, Props, VIZ_TEST_FILE_NAME, &Ret);
    CHECK_ERR(Ret, CL_INVALID_VALUE);
    if (Dot != nullptr) {
      throw std::runtime_error("NULL handle not returned on error");
    }
  }

  // Invalid duplicate property name
  {
    cl_dot_graph_flags_ext Flags = CL_DOT_GRAPH_COLOR_EXT;
    cl_dot_graph_properties_ext Props[] = {CL_DOT_GRAPH_FLAGS_EXT, Flags,
                                           CL_DOT_GRAPH_FLAGS_EXT, 0, 0};
    cl_int Ret = CL_SUCCESS;
    cl_dot_graph_ext Dot = State.clCreateDotGraphEXT(
        1, &State.InOrderQueue, Props, VIZ_TEST_FILE_NAME, &Ret);
    CHECK_ERR(Ret, CL_INVALID_VALUE);
    if (Dot != nullptr) {
      throw std::runtime_error("NULL handle not returned on error");
    }
  }

  return 0;
}
