// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -DVIZ_TEST_FILE_NAME=\"%T/%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_1[label="clEnqueueTask"];
// CHECK-NEXT: node_2[label="clEnqueueTask"];
// CHECK-NEXT: }
// CHECK-NEXT: node_1 -> node_2
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State(true);

  cl_int Ret = CL_SUCCESS;
  cl_dot_graph_ext Dot1 = State.clCreateDotGraphEXT(
      1, &State.InOrderQueue, nullptr, VIZ_TEST_FILE_NAME, &Ret);
  CHECK(Ret);

  // Create single node dot graph, then overwrite.
  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clReleaseDotGraphEXT(Dot1);
  CHECK(Ret);

  Dot1 = State.clCreateDotGraphEXT(1, &State.InOrderQueue, nullptr,
                                   VIZ_TEST_FILE_NAME, &Ret);
  CHECK(Ret);

  Ret = clEnqueueTask(State.InOrderQueue, State.Kernel, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueTask(State.InOrderQueue, State.Kernel, 0, nullptr, nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.InOrderQueue));

  Ret = State.clReleaseDotGraphEXT(Dot1);
  CHECK(Ret);

  return 0;
}
