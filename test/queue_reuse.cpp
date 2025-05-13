// Copyright (c) 2025 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: }
// CHECK-NEXT: cluster_1 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_1[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1 [label="blocking host wait"
// CHECK-SAME: ltail=cluster_0 lhead=cluster_1];
// CHECK-NEXT: cluster_2 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_2[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: }
// CHECK-NEXT: node_1 -> node_2 [label="blocking host wait"
// CHECK-SAME: ltail=cluster_1 lhead=cluster_2];
// CHECK-NEXT: cluster_3 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_3[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: }
// CHECK-NEXT: node_2 -> node_3 [label="blocking host wait"
// CHECK-SAME: ltail=cluster_2 lhead=cluster_3];
// CHECK-NEXT: cluster_4 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_4[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: }
// CHECK-NEXT: node_3 -> node_4 [label="blocking host wait"
// CHECK-SAME: ltail=cluster_3 lhead=cluster_4];
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"
#include <array>

int main() {
  CLState State;

  constexpr unsigned N = 5;
  for (unsigned i = 0; i < N; i++) {
    cl_int Ret = CL_SUCCESS;
    cl_command_queue Queue =
        clCreateCommandQueue(State.Context, State.Device, 0, &Ret);
    CHECK(Ret);

    Ret =
        clEnqueueNDRangeKernel(Queue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);

    Ret = clFinish(Queue);
    CHECK(Ret);

    clReleaseCommandQueue(Queue);
  }

  return 0;
}
