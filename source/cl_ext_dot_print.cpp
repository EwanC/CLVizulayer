// Copyright (c) 2025-2026 Ewan Crawford

#define CL_ENABLE_BETA_EXTENSIONS
#include "cl_ext_dot_print.h"

#include <atomic>
#include <span>

#include "logger.h"
#include "viz_context.h"

/// Forward declaration
struct VizDotFile;

/// @brief Object that underpins user returned `cl_dot_graph_ext` pointer
struct _cl_dot_graph_ext {
  /// @brief Constructor
  /// @param CLQueueList List of queues to record
  /// @param FilePath User specified path to dot file
  _cl_dot_graph_ext(std::span<const cl_command_queue> CLQueueList,
                    const cl_dot_graph_flags_ext Flags, const char *FilePath)
      : MRefCount(1) {
    auto &Context = viz::getVizContext();
    MInstance = Context.createVizInstance(CLQueueList, Flags, FilePath);
  }

  /// @brief Increment reference count
  cl_int retain() {
    MRefCount.fetch_add(1, std::memory_order_relaxed);
    return CL_SUCCESS;
  }

  /// @brief Decrement reference conut, and destroy object if zero
  cl_int release() {
    MRefCount.fetch_sub(1, std::memory_order_relaxed);
    if (MRefCount.load(std::memory_order_relaxed) == 0) {
      // VizContext owns the VizInstance allocation
      auto &Context = viz::getVizContext();
      Context.destroyVizInstance(MInstance);

      delete this;
    }
    return CL_SUCCESS;
  }

private:
  /// @brief Reference Count
  std::atomic<uint32_t> MRefCount;
  /// @brief Underlying Viz Instance object that does all the work.
  VizInstance *MInstance;
};

CL_API_ENTRY cl_dot_graph_ext CL_API_CALL
clCreateDotGraphEXT(cl_uint num_queues, const cl_command_queue *queues,
                    const cl_dot_graph_properties_ext *properties,
                    const char *file_path, cl_int *errcode_ret) {
  cl_dot_graph_flags_ext Flags = 0;
  if (properties) {
    const cl_dot_graph_properties_ext *CurrProp = properties;
    bool SeenFlags = false;
    while (CurrProp[0] != 0) {
      if (CurrProp[0] != CL_DOT_GRAPH_FLAGS_EXT || SeenFlags) {
        if (errcode_ret) {
          *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
      }
      Flags = *((const cl_dot_graph_flags_ext *)(CurrProp + 1));
      cl_dot_graph_flags_ext ValidBits =
          CL_DOT_GRAPH_COLOR_EXT | CL_DOT_GRAPH_VERBOSE_EXT;
      if (Flags > ValidBits) {
        if (errcode_ret) {
          *errcode_ret = CL_INVALID_VALUE;
        }
        return NULL;
      }
      SeenFlags = true;
      CurrProp += 2;
    }
  }

  if (queues == nullptr) {
    if (errcode_ret) {
      *errcode_ret = CL_INVALID_VALUE;
    }
    return NULL;
  }
  std::span<const cl_command_queue> QueueList(queues, num_queues);
  cl_context ListContext = nullptr;
  cl_int QueueErr = CL_SUCCESS;

  for (auto &Queue : QueueList) {
    if (Queue == nullptr) {
      QueueErr = CL_INVALID_COMMAND_QUEUE;
      break;
    }
    cl_context QueueContext;
    auto Ret = TargetDispatch->clGetCommandQueueInfo(
        Queue, CL_QUEUE_CONTEXT, sizeof(cl_context), &QueueContext, NULL);
    if (Ret != CL_SUCCESS) {
      QueueErr = CL_INVALID_COMMAND_QUEUE;
      break;
    }
    if (!ListContext) {
      ListContext = QueueContext;
    } else if (ListContext != QueueContext) {
      QueueErr = CL_INVALID_CONTEXT;
      break;
    }
  }

  if (QueueErr != CL_SUCCESS) {
    if (errcode_ret) {
      *errcode_ret = QueueErr;
    }
    return NULL;
  }

  cl_dot_graph_ext RetPtr = NULL;
  try {
    RetPtr = new _cl_dot_graph_ext(QueueList, Flags, file_path);
    if (RetPtr != NULL && errcode_ret) {
      *errcode_ret = CL_SUCCESS;
    }
  } catch (std::exception &E) {
    VIZ_ERR("Error instantiating _cl_dot_graph_ext: {}", E.what());
    if (errcode_ret) {
      *errcode_ret = CL_OUT_OF_HOST_MEMORY;
    }
  }
  return RetPtr;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainDotGraphEXT(cl_dot_graph_ext dot) {
  return dot ? dot->retain() : CL_INVALID_VALUE;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseDotGraphEXT(cl_dot_graph_ext dot) {
  return dot ? dot->release() : CL_INVALID_VALUE;
}
