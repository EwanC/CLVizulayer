// Copyright (c) 2025 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t 2> %T/%basename_t.cerr
// RUN: FileCheck --input_file %T/%basename_t.cerr %s

// CHECK:Warning clEnqueueCommandBufferKHR not tracked in dot graph layer

#include "common.h"

#define CL_ENABLE_BETA_EXTENSIONS
#include <CL/cl_ext.h>

int main() {
  CLState State;

  auto Func = reinterpret_cast<clEnqueueCommandBufferKHR_fn>(
      clGetExtensionFunctionAddressForPlatform(State.PlatformId,
                                               "clEnqueueCommandBufferKHR"));

  return 0;
}
