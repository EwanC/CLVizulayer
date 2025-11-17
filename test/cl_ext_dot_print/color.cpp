// Copyright (c) 2025 Ewan Crawford

// RUN: %build -DVIZ_TEST_FILE_NAME=\"%T/%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=filled]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "Viz instance teardown";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="aliceblue"];
// CHECK-NEXT: node_1[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="aqua"];
// CHECK-NEXT: node_2[label="clEnqueueNDRangeKernel",
// CHECK-SAME: color="aliceblue"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: node_1 -> node_2
// CHECK-NEXT: node_0 -> node_2
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State(true);

  cl_dot_graph_flags_ext Flags = CL_DOT_GRAPH_COLOR_EXT;
  cl_dot_graph_properties_ext Props[3] = {CL_DOT_GRAPH_FLAGS_EXT, Flags, 0};

  cl_command_queue Queues[2] = {State.InOrderQueue, State.OutOfOrderQueue};
  cl_int Ret = CL_SUCCESS;
  cl_dot_graph_ext Dot =
      State.clCreateDotGraphEXT(2, Queues, Props, VIZ_TEST_FILE_NAME, &Ret);
  CHECK(Ret);

  cl_event Events[2] = {nullptr, nullptr};
  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr,
                               &Events[0]);

  Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 1, &Events[0],
                               &Events[1]);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 1, &Events[1],
                               nullptr);
  CHECK(Ret);

  Ret = State.clReleaseDotGraphEXT(Dot);
  CHECK(Ret);

  CHECK(clFlush(State.OutOfOrderQueue));
  CHECK(clFinish(State.InOrderQueue));

  CHECK(clReleaseEvent(Events[0]));
  CHECK(clReleaseEvent(Events[1]));

  return 0;
}
