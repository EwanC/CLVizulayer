// Copyright (c) 2025-2026 Ewan Crawford

// REQUIRES: svm-support

// RUN: %build -o %t
// RUN: VIZ_VERBOSE=1 VIZ_DOT_FILE=%T/%basename_t.dot %t
// RUN: FileCheck --input_file %T/%basename_t.dot %s

// CHECK: digraph CLVizulayer {
// CHECK-NEXT: compound=true
// CHECK-NEXT: node [style=bold]
// CHECK-NEXT: subgraph cluster_0 {
// CHECK-NEXT: label = "clFinish()";
// CHECK-NEXT: node_0[label="clEnqueueSVMFree
// CHECK-NEXT: svm_ptr = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: svm_ptr = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: pfn_free_func = 0x{{[0-9a-fA-F]+}}
// CHECK-NEXT: user_data = 0x{{[0-9a-fA-F]+}}"];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: }

#include "../common.h"
#include <cassert>

struct CallbackData {
  void **FreedPtrs[2];
};

void CL_CALLBACK callbackSVMFree(cl_command_queue Queue, cl_uint NumSVMPtrs,
                                 void *SVMPtrs[], void *UserData) {

  cl_context Context;
  cl_int Ret = clGetCommandQueueInfo(Queue, CL_QUEUE_CONTEXT,
                                     sizeof(cl_context), &Context, 0);
  CHECK(Ret);

  CallbackData *Data = (CallbackData *)UserData;
  for (size_t i = 0; i < NumSVMPtrs; ++i) {
    clSVMFree(Context, SVMPtrs[i]);
    // Avoid double free in State teardown
    *(Data->FreedPtrs[i]) = nullptr;
  }
}

int main() {
  CLState State;

  void *SVMPtrs[2] = {State.SVMA, State.SVMB};
  CallbackData UserData{{&State.SVMA, &State.SVMB}};
  cl_int Ret =
      clEnqueueSVMFree(State.OutOfOrderQueue, 2, SVMPtrs, callbackSVMFree,
                       &UserData, 0, nullptr, nullptr);
  CHECK(Ret);

  CHECK(clFinish(State.OutOfOrderQueue));

  assert(State.SVMA == nullptr);
  assert(State.SVMB == nullptr);

  return 0;
}
