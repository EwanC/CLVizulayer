// Copyright (c) 2025-2026 Ewan Crawford

#pragma once

#define CL_ENABLE_BETA_EXTENSIONS
#include <CL/cl_ext.h>
#include <stdexcept>
#include <string>
#include <vector>

#include "cl_ext_command_buffer_dot_print.h"
#include "cl_ext_dot_print.h"

#define CHECK(Ret)                                                             \
  if (Ret != CL_SUCCESS) {                                                     \
    throw std::runtime_error(                                                  \
        std::string("OpenCL error " + std::to_string(Ret)));                   \
  }

#define CHECK_ERR(Ret, ErrCode)                                                \
  if (Ret != ErrCode) {                                                        \
    throw std::runtime_error(                                                  \
        std::string("Expected OpenCL return code " + std::to_string(ErrCode) + \
                    ", but got return code " + std::to_string(Ret)));          \
  }

#define CHECK_NOT_NULL(Ptr)                                                    \
  if (Ptr == nullptr) {                                                        \
    throw std::runtime_error(std::string("Unexpected nullptr"));               \
  }

struct CLState {
  CLState(bool ExtensionEnabled = false);
  ~CLState();

  static const std::string KernelSource;
  static const size_t GlobalSize;
  static const size_t AllocSize;
  static const size_t ImgHeight;

  cl_platform_id PlatformId = NULL;
  cl_device_id Device = nullptr;
  cl_context Context = nullptr;
  cl_command_queue InOrderQueue = nullptr;
  cl_command_queue OutOfOrderQueue = nullptr;
  cl_program Program = nullptr;
  cl_kernel Kernel = nullptr;
  cl_mem BufferA = nullptr;
  cl_mem BufferB = nullptr;
  cl_mem ImageA = nullptr;
  cl_mem ImageB = nullptr;
  void *SVMA = nullptr;
  void *SVMB = nullptr;

  clCreateDotGraphEXT_fn clCreateDotGraphEXT = nullptr;
  clReleaseDotGraphEXT_fn clReleaseDotGraphEXT = nullptr;
  clRetainDotGraphEXT_fn clRetainDotGraphEXT = nullptr;
  clDotPrintCommandBufferEXT_fn clDotPrintCommandBufferEXT = nullptr;

  clCreateCommandBufferKHR_fn clCreateCommandBufferKHR = nullptr;
  clReleaseCommandBufferKHR_fn clReleaseCommandBufferKHR = nullptr;
  clCommandBarrierWithWaitListKHR_fn clCommandBarrierWithWaitListKHR = nullptr;
  clCommandCopyBufferKHR_fn clCommandCopyBufferKHR = nullptr;
  clCommandCopyBufferRectKHR_fn clCommandCopyBufferRectKHR = nullptr;
  clCommandCopyBufferToImageKHR_fn clCommandCopyBufferToImageKHR = nullptr;
  clCommandCopyImageKHR_fn clCommandCopyImageKHR = nullptr;
  clCommandCopyImageToBufferKHR_fn clCommandCopyImageToBufferKHR = nullptr;
  clCommandFillBufferKHR_fn clCommandFillBufferKHR = nullptr;
  clCommandFillImageKHR_fn clCommandFillImageKHR = nullptr;
  clCommandNDRangeKernelKHR_fn clCommandNDRangeKernelKHR = nullptr;
  clEnqueueCommandBufferKHR_fn clEnqueueCommandBufferKHR = nullptr;
  clFinalizeCommandBufferKHR_fn clFinalizeCommandBufferKHR = nullptr;
};

CLState::CLState(bool ExtensionEnabled) {
  cl_int Ret = clGetPlatformIDs(1, &PlatformId, nullptr);
  CHECK(Ret);

  Ret = clGetDeviceIDs(PlatformId, CL_DEVICE_TYPE_DEFAULT, 1, &Device, nullptr);
  CHECK(Ret);

  Context = clCreateContext(NULL, 1, &Device, NULL, NULL, &Ret);
  CHECK(Ret);

  InOrderQueue = clCreateCommandQueue(Context, Device, 0, &Ret);
  CHECK(Ret);

  cl_queue_properties Props[] = {CL_QUEUE_PROPERTIES,
                                 CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, 0};
  OutOfOrderQueue =
      clCreateCommandQueueWithProperties(Context, Device, Props, &Ret);
  CHECK(Ret);

  const char *SourceCStr = KernelSource.c_str();
  Program = clCreateProgramWithSource(Context, 1, &SourceCStr, nullptr, &Ret);
  CHECK(Ret);

  Ret = clBuildProgram(Program, 1, &Device, NULL, NULL, NULL);
  CHECK(Ret);

  Kernel = clCreateKernel(Program, "no_op", &Ret);
  CHECK(Ret);

  BufferA = clCreateBuffer(Context, 0, AllocSize, NULL, &Ret);
  CHECK(Ret);

  BufferB = clCreateBuffer(Context, 0, AllocSize, NULL, &Ret);
  CHECK(Ret);

  cl_bool image_support;
  Ret = clGetDeviceInfo(Device, CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool),
                        &image_support, NULL);
  CHECK(Ret);
  if (image_support) {
    cl_image_desc Desc{};
    Desc.image_type = CL_MEM_OBJECT_IMAGE2D;
    Desc.image_height = Desc.image_width = AllocSize;
    cl_image_format Format{CL_RGBA, CL_FLOAT};
    ImageA = clCreateImage(Context, 0, &Format, &Desc, NULL, &Ret);
    CHECK(Ret);
    ImageB = clCreateImage(Context, 0, &Format, &Desc, NULL, &Ret);
    CHECK(Ret);
  }

  cl_device_svm_capabilities svm_caps;
  Ret = clGetDeviceInfo(Device, CL_DEVICE_SVM_CAPABILITIES, sizeof(svm_caps),
                        &svm_caps, NULL);
  CHECK(Ret);
  if (svm_caps & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER) {
    SVMA = clSVMAlloc(Context, CL_MEM_READ_WRITE, AllocSize,
                      0 /* default align */);
    SVMB = clSVMAlloc(Context, CL_MEM_READ_WRITE, AllocSize,
                      0 /* default align */);
    if (!SVMA || !SVMB) {
      throw std::runtime_error(
          std::string("OpenCL error: clSVMAlloc failed" + std::to_string(Ret)));
    }
  }

#define GET_EXTENSION_ADDRESS(FUNC)                                            \
  FUNC = reinterpret_cast<FUNC##_fn>(                                          \
      clGetExtensionFunctionAddressForPlatform(PlatformId, #FUNC));            \
  if (FUNC == nullptr) {                                                       \
    throw std::runtime_error(                                                  \
        "ERROR: clGetExtensionFunctionAddressForPlatform failed"               \
        " with " #FUNC "\n");                                                  \
  }

  if (ExtensionEnabled) {
    GET_EXTENSION_ADDRESS(clCreateDotGraphEXT);
    GET_EXTENSION_ADDRESS(clReleaseDotGraphEXT);
    GET_EXTENSION_ADDRESS(clRetainDotGraphEXT);
  }

  size_t Size = 0;
  Ret = clGetDeviceInfo(Device, CL_DEVICE_EXTENSIONS, 0, nullptr, &Size);
  CHECK(Ret);

  std::vector<char> DeviceExtensions(Size);
  Ret = clGetDeviceInfo(Device, CL_DEVICE_EXTENSIONS, Size,
                        DeviceExtensions.data(), nullptr);
  CHECK(Ret);

  if (std::string ExtensionStr(DeviceExtensions.data());
      ExtensionStr.find(CL_KHR_COMMAND_BUFFER_EXTENSION_NAME) !=
      std::string::npos) {
    if (ExtensionEnabled) {
      GET_EXTENSION_ADDRESS(clDotPrintCommandBufferEXT);
    }
    GET_EXTENSION_ADDRESS(clCreateCommandBufferKHR);
    GET_EXTENSION_ADDRESS(clReleaseCommandBufferKHR);
    GET_EXTENSION_ADDRESS(clCommandBarrierWithWaitListKHR);
    GET_EXTENSION_ADDRESS(clCommandCopyBufferKHR);
    GET_EXTENSION_ADDRESS(clCommandCopyBufferRectKHR);
    GET_EXTENSION_ADDRESS(clCommandCopyBufferToImageKHR);
    GET_EXTENSION_ADDRESS(clCommandCopyImageKHR);
    GET_EXTENSION_ADDRESS(clCommandCopyImageToBufferKHR);
    GET_EXTENSION_ADDRESS(clCommandFillBufferKHR);
    GET_EXTENSION_ADDRESS(clCommandFillImageKHR);
    GET_EXTENSION_ADDRESS(clCommandNDRangeKernelKHR);
    GET_EXTENSION_ADDRESS(clEnqueueCommandBufferKHR);
    GET_EXTENSION_ADDRESS(clFinalizeCommandBufferKHR);
  }
#undef GET_EXTENSION_FUNCTION_ADDRESS
}

CLState::~CLState() {
  if (InOrderQueue) {
    clReleaseCommandQueue(InOrderQueue);
  }
  if (OutOfOrderQueue) {
    clReleaseCommandQueue(OutOfOrderQueue);
  }
  if (Kernel) {
    clReleaseKernel(Kernel);
  }
  if (Program) {
    clReleaseProgram(Program);
  }
  if (BufferA) {
    clReleaseMemObject(BufferA);
  }
  if (BufferB) {
    clReleaseMemObject(BufferB);
  }
  if (ImageA) {
    clReleaseMemObject(ImageA);
  }
  if (ImageB) {
    clReleaseMemObject(ImageB);
  }
  if (SVMA) {
    clSVMFree(Context, SVMA);
  }
  if (SVMB) {
    clSVMFree(Context, SVMB);
  }
  if (Context) {
    clReleaseContext(Context);
  }
}

const std::string CLState::KernelSource = std::string("kernel void no_op() {}");
const size_t CLState::GlobalSize = 1;
const size_t CLState::AllocSize = 32;
