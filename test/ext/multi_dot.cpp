// Copyright (c) 2025 Ewan Crawford

// RUN: %build -DVIZ_TEST_FILE_NAME1=\"%T/1_%basename_t.dot\" \
// RUN: -DVIZ_TEST_FILE_NAME2=\"%T/2_%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t
// RUN: FileCheck --check-prefix CHECK1 --input_file %T/1_%basename_t.dot %s
// RUN: FileCheck --check-prefix CHECK2 --input_file %T/2_%basename_t.dot %s

// CHECK1: digraph CLVizulayer {
// CHECK1-NEXT: compound=true
// CHECK1-NEXT: node [style=bold]
// CHECK1-NEXT: subgraph cluster_0 {
// CHECK1-NEXT: label = "Viz instance teardown";
// CHECK1-NEXT: node_0[label="clEnqueueNDRangeKernel"];
// CHECK1-NEXT: node_1[label="clEnqueueTask"];
// CHECK1-NEXT: }
// CHECK1-NEXT: node_0 -> node_1
// CHECK1-EMPTY:
// CHECK1-NEXT: }

// CHECK2: digraph CLVizulayer {
// CHECK2-NEXT: compound=true
// CHECK2-NEXT: node [style=bold]
// CHECK2-NEXT: subgraph cluster_0 {
// CHECK2-NEXT: label = "clFinish()";
// CHECK2-NEXT: node_2[label="clEnqueueTask"];
// CHECK2-NEXT: node_3[label="clEnqueueNDRangeKernel"];
// CHECK2-NEXT: }
// CHECK2-NEXT: node_2 -> node_3
// CHECK2-EMPTY:
// CHECK2-NEXT: }

#include "../common.h"

int main() {
  CLState State(true);

  cl_int Ret = CL_SUCCESS;
  cl_dot_graph_ext Dot1 = State.clCreateDotGraphEXT(1, &State.InOrderQueue,
                                                    VIZ_TEST_FILE_NAME1, &Ret);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  cl_dot_graph_ext Dot2 = State.clCreateDotGraphEXT(1, &State.InOrderQueue,
                                                    VIZ_TEST_FILE_NAME2, &Ret);
  CHECK(Ret);

  Ret = clEnqueueTask(State.InOrderQueue, State.Kernel, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clReleaseDotGraphEXT(Dot1);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.InOrderQueue));

  Ret = State.clReleaseDotGraphEXT(Dot2);
  CHECK(Ret);

  return 0;
}
