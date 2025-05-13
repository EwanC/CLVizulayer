// Copyright (c) 2025 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel
// CHECK-NEXT: cl_kernel = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: name = no_op"];
// CHECK-NEXT: node_1[label="clEnqueueMigrateMemObjects
// CHECK-NEXT: mem_obj = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: mem_obj = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: flags = 1"];
// CHECK-NEXT: node_2[label="clEnqueueNDRangeKernel
// CHECK-NEXT: cl_kernel = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: name = no_op"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: node_1 -> node_2
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"

int main() {
  CLState State;

  cl_event Events[2] = {nullptr, nullptr};

  cl_int Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1,
                                      nullptr, &State.GlobalSize, nullptr, 0,
                                      nullptr, &Events[0]);
  CHECK(Ret);

  constexpr cl_uint NumMems = 2;
  cl_mem Mems[NumMems] = {State.BufferA, State.BufferB};
  cl_mem_migration_flags Flags = CL_MIGRATE_MEM_OBJECT_HOST;
  Ret = clEnqueueMigrateMemObjects(State.OutOfOrderQueue, NumMems, Mems, Flags,
                                   1, &Events[0], &Events[1]);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.OutOfOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 1, &Events[1],
                               nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.OutOfOrderQueue));
  CHECK(clReleaseEvent(Events[0]));
  CHECK(clReleaseEvent(Events[1]));
  return 0;
}
