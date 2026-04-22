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
  cl_int Ret =
      clGetDeviceInfo(State.Device, CL_DEVICE_EXTENSIONS, 0, nullptr, &StrSize);
  CHECK(Ret);

  std::vector<char> DeviceExtensions(StrSize, 0);
  Ret = clGetDeviceInfo(State.Device, CL_DEVICE_EXTENSIONS, StrSize,
                        DeviceExtensions.data(), nullptr);
  CHECK(Ret);

  std::string DeviceExtString(DeviceExtensions.data());
  std::cout << DeviceExtString << std::endl;

  return 0;
}
