// Copyright (c) 2025 Ewan Crawford

// RUN: %build -o %t
// RUN: cd %T && VIZ_EXT=1 %t
// We don't know the file location to verify with FILECHECK
#include "../common.h"

int main() {
  CLState State(true);

  cl_int Ret = CL_SUCCESS;
  cl_dot_graph_ext Dot =
      State.clCreateDotGraphEXT(1, &State.InOrderQueue, nullptr, nullptr, &Ret);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.InOrderQueue));

  Ret = State.clReleaseDotGraphEXT(Dot);
  CHECK(Ret);

  return 0;
}
