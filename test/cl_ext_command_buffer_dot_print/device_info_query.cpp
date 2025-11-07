// Copyright (c) 2025 Ewan Crawford

// REQUIRES: command-buffer

// RUN: %build -o %t
// RUN: %t

#include "../common.h"
#include <cstring>
#include <vector>

int main() {
  CLState State;

  size_t StrSize;
  cl_int Ret =
      clGetDeviceInfo(State.Device, CL_DEVICE_EXTENSIONS, 0, nullptr, &StrSize);
  CHECK(Ret);

  std::vector<char> DeviceExtensions(StrSize, 0);
  Ret = clGetDeviceInfo(State.Device, CL_DEVICE_EXTENSIONS, StrSize,
                        DeviceExtensions.data(), nullptr);
  CHECK(Ret);

  std::string DeviceExtString(DeviceExtensions.data());
  if (DeviceExtString.find(CL_KHR_COMMAND_BUFFER_EXTENSION_NAME) ==
      std::string::npos) {
    // We only return command-buffer extension here if cl_khr_command_buffer is
    // also reported as a device extension (it may have been reported as a
    // platform extension instead).
    return 0;
  }

  const char *CBDotGraphExtName = "cl_ext_command_buffer_dot_graph";
  if (DeviceExtString.find(CBDotGraphExtName) == std::string::npos) {
    throw std::runtime_error("Could not find cl_ext_command_buffer_dot_graph "
                             "in CL_DEVICE_EXTENSIONS");
  }

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
    if (strcmp(Ext.name, CBDotGraphExtName) == 0) {
      if (Ext.version != CL_MAKE_VERSION(0, 1, 0)) {
        throw std::runtime_error(
            "Expected cl_ext_command_buffer_dot_graph version 0.1.0");
      }

      Found = true;
      break;
    }
  }

  if (!Found) {
    throw std::runtime_error("Could not find cl_ext_command_buffer_dot_graph "
                             "in CL_DEVICE_EXTENSIONS_WITH_VERSION");
  }

  return 0;
}
