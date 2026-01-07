// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -DVIZ_TEST_FILE_NAME=\"%T/%basename_t.dot\" -o %t
// RUN: VIZ_EXT=1 %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "Viz instance teardown";
// CHECK-NEXT: node_0[label="clEnqueueCopyBuffer
// CHECK-NEXT: src_buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: dst_buffer = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: src_offset = 1
// CHECK-NEXT: dst_offset = 1
// CHECK-NEXT: size = 31"];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State(true);

  cl_dot_graph_flags_ext Flags = CL_DOT_GRAPH_VERBOSE_EXT;
  cl_dot_graph_properties_ext Props[3] = {CL_DOT_GRAPH_FLAGS_EXT, Flags, 0};

  cl_int Ret = CL_SUCCESS;
  cl_dot_graph_ext Dot = State.clCreateDotGraphEXT(
      1, &State.OutOfOrderQueue, Props, VIZ_TEST_FILE_NAME, &Ret);
  CHECK(Ret);

  Ret = clEnqueueCopyBuffer(State.OutOfOrderQueue, State.BufferA, State.BufferB,
                            1, 1, State.AllocSize - 1, 0, nullptr, nullptr);
  CHECK(Ret);

  Ret = State.clReleaseDotGraphEXT(Dot);
  CHECK(Ret);

  return 0;
}
