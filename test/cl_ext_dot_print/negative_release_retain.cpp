// Copyright (c) 2025 Ewan Crawford

// RUN: %build -DVIZ_TEST_FILE_NAME=\"%T/%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t

#include "../common.h"

int main() {
  CLState State(true);

  cl_int Ret = State.clRetainDotGraphEXT(nullptr);
  CHECK_ERR(Ret, CL_INVALID_VALUE);

  Ret = State.clReleaseDotGraphEXT(nullptr);
  CHECK_ERR(Ret, CL_INVALID_VALUE);

  return 0;
}
