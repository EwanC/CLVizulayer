// Copyright (c) 2025 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "Viz instance teardown";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_1[label="clEnqueueTask"];
// CHECK-NEXT: node_2[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: node_1 -> node_2
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"

int main() {
  cl_platform_id PlatformId = nullptr;
  cl_int Ret = clGetPlatformIDs(1, &PlatformId, nullptr);
  CHECK(Ret);

  cl_device_id Device = nullptr;
  Ret = clGetDeviceIDs(PlatformId, CL_DEVICE_TYPE_DEFAULT, 1, &Device, nullptr);
  CHECK(Ret);

  cl_context Context =
      clCreateContext(nullptr, 1, &Device, nullptr, nullptr, &Ret);
  CHECK(Ret);

  cl_command_queue InOrderQueue =
      clCreateCommandQueue(Context, Device, 0, &Ret);
  CHECK(Ret);

  const char *SourceCStr = "kernel void no_op() {}";
  cl_program Program =
      clCreateProgramWithSource(Context, 1, &SourceCStr, nullptr, &Ret);
  CHECK(Ret);

  Ret = clBuildProgram(Program, 1, &Device, nullptr, nullptr, nullptr);
  CHECK(Ret);

  cl_kernel Kernel = clCreateKernel(Program, "no_op", &Ret);
  CHECK(Ret);

  size_t GlobalSize = 1;
  Ret = clEnqueueNDRangeKernel(InOrderQueue, Kernel, 1, nullptr, &GlobalSize,
                               nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueTask(InOrderQueue, Kernel, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(InOrderQueue, Kernel, 1, nullptr, &GlobalSize,
                               nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  // Return without doing a blocking wait or releasing objects
  return 0;
}
