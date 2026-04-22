// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -o %t
// RUN: %t | FileCheck %s

// CHECK-NOT: cl_ext_dot_graph

#include "../common.h"
#include <iostream>
#include <vector>

int main() {
  CLState State(false);

  size_t StrSize;
  cl_int Ret = clGetPlatformInfo(State.PlatformId, CL_PLATFORM_EXTENSIONS, 0,
                                 nullptr, &StrSize);
  CHECK(Ret);

  std::vector<char> PlatformExtensions(StrSize, 0);
  Ret = clGetPlatformInfo(State.PlatformId, CL_PLATFORM_EXTENSIONS, StrSize,
                          PlatformExtensions.data(), nullptr);
  CHECK(Ret);

  std::string PlatformExtString(PlatformExtensions.data());
  std::cout << PlatformExtString << std::endl;

  return 0;
}
