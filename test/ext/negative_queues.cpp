// Copyright (c) 2025 Ewan Crawford

// RUN: %build -DVIZ_TEST_FILE_NAME=\"%T/%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t

#include "../common.h"

int main() {
  CLState State(true);

  cl_int Ret = CL_SUCCESS;
  cl_dot_graph_ext Dot =
      State.clCreateDotGraphEXT(0, nullptr, VIZ_TEST_FILE_NAME, &Ret);
  CHECK_ERR(Ret, CL_INVALID_VALUE);
  if (Dot != nullptr) {
    throw std::runtime_error("NULL handle not returned on error");
  }

  cl_command_queue BadQueue = nullptr;
  Dot = State.clCreateDotGraphEXT(1, &BadQueue, VIZ_TEST_FILE_NAME, &Ret);
  CHECK_ERR(Ret, CL_INVALID_COMMAND_QUEUE);
  if (Dot != nullptr) {
    throw std::runtime_error("NULL handle not returned on error");
  }

  cl_context SecondContext =
      clCreateContext(0, 1, &State.Device, nullptr, nullptr, &Ret);
  CHECK(Ret);

  cl_command_queue OtherCtxQueue =
      clCreateCommandQueue(SecondContext, State.Device, 0, &Ret);
  CHECK(Ret);

  cl_command_queue MismatchQueues[2] = {State.InOrderQueue, OtherCtxQueue};
  Dot = State.clCreateDotGraphEXT(2, MismatchQueues, VIZ_TEST_FILE_NAME, &Ret);
  CHECK_ERR(Ret, CL_INVALID_CONTEXT);
  if (Dot != nullptr) {
    throw std::runtime_error("NULL handle not returned on error");
  }

  CHECK(clReleaseContext(SecondContext));

  return 0;
}
