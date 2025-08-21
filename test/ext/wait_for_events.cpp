// Copyright (c) 2025 Ewan Crawford

// RUN: %build -DVIZ_TEST_FILE_NAME=\"%T/%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clWaitForEvents()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_1[label="clEnqueueWriteBuffer"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State(true);

  cl_int Ret = CL_SUCCESS;
  cl_dot_graph_ext Dot = State.clCreateDotGraphEXT(1, &State.InOrderQueue,
                                                   VIZ_TEST_FILE_NAME, &Ret);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  cl_event Events[3] = {nullptr, nullptr, nullptr};
  Ret = clEnqueueTask(State.OutOfOrderQueue, State.Kernel, 0, nullptr,
                      &Events[0]);
  CHECK(Ret);

  cl_int Foo = 0xA;
  Ret = clEnqueueWriteBuffer(State.OutOfOrderQueue, State.BufferA, CL_FALSE, 0,
                             sizeof(Foo), &Foo, 1, &Events[0], &Events[1]);
  CHECK(Ret);

  CHECK(clWaitForEvents(2, Events));

  Foo = 42;
  Ret = clEnqueueWriteBuffer(State.InOrderQueue, State.BufferA, CL_FALSE, 0,
                             sizeof(Foo), &Foo, 0, nullptr, &Events[2]);
  CHECK(Ret);

  CHECK(clWaitForEvents(1, &Events[2]));

  Ret = State.clReleaseDotGraphEXT(Dot);
  CHECK(Ret);

  CHECK(clReleaseEvent(Events[0]));
  CHECK(clReleaseEvent(Events[1]));
  CHECK(clReleaseEvent(Events[2]));

  return 0;
}
