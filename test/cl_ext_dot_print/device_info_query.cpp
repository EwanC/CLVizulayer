// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_EXT=1 %t | FileCheck %s

// CHECK: cl_ext_dot_graph

#include "../common.h"
#include <cstring>
#include <iostream>
#include <vector>

int main() {
  CLState State(true);

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

  size_t ExtWithVerSize;
  Ret = clGetDeviceInfo(State.Device, CL_DEVICE_EXTENSIONS_WITH_VERSION, 0,
                        nullptr, &ExtWithVerSize);
  CHECK(Ret);

  std::vector<cl_name_version> DeviceExtWithVer(ExtWithVerSize /
                                                sizeof(cl_name_version));
  Ret = clGetDeviceInfo(State.Device, CL_DEVICE_EXTENSIONS_WITH_VERSION,
                        ExtWithVerSize, DeviceExtWithVer.data(), nullptr);
  CHECK(Ret);

  bool Found = false;
  for (const auto &Ext : DeviceExtWithVer) {
    if (strcmp(Ext.name, "cl_ext_dot_graph") == 0) {
      if (Ext.version != CL_MAKE_VERSION(0, 1, 0)) {
        throw std::runtime_error("Expected cl_ext_dot_graph version 0.1.0");
      }

      Found = true;
      break;
    }
  }

  if (!Found) {
    throw std::runtime_error(
        "Could not find cl_ext_dot_graph in CL_DEVICE_EXTENSIONS_WITH_VERSION");
  }

  return 0;
}
