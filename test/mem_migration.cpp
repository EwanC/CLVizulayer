// Copyright (c) 2025-2026 Ewan Crawford

// RUN: %build -o %t
// RUN: VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_0[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: node_1[label="clEnqueueMigrateMemObjects"];
// CHECK-NEXT: node_2[label="clEnqueueNDRangeKernel"];
// CHECK-NEXT: }
// CHECK-NEXT: node_0 -> node_1
// CHECK-NEXT: node_1 -> node_2
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "common.h"

int main() {
  CLState State;

  cl_int Ret =
      clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                             &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  constexpr cl_uint NumMems = 2;
  cl_mem Mems[NumMems] = {State.BufferA, State.BufferB};
  cl_mem_migration_flags Flags = CL_MIGRATE_MEM_OBJECT_HOST;
  Ret = clEnqueueMigrateMemObjects(State.InOrderQueue, NumMems, Mems, Flags, 0,
                                   nullptr, nullptr);
  CHECK(Ret);

  Ret = clEnqueueNDRangeKernel(State.InOrderQueue, State.Kernel, 1, nullptr,
                               &State.GlobalSize, nullptr, 0, nullptr, nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.InOrderQueue));
  return 0;
}
