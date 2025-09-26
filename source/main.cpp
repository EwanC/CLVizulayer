// Copyright (c) 2025 Ewan Crawford

#include "io.h"
#include "logger.h"
#include "viz.h"
#include <cstring>
#include <iostream>

#define CL_ENABLE_BETA_EXTENSIONS
#include "cl_ext_dot_graph.h"

CL_API_ENTRY cl_int CL_API_CALL clGetLayerInfo(cl_layer_info param_name,
                                               size_t param_value_size,
                                               void *param_value,
                                               size_t *param_value_size_ret) {
  switch (param_name) {
  case CL_LAYER_API_VERSION:
    if (param_value) {
      if (param_value_size < sizeof(cl_layer_api_version))
        return CL_INVALID_VALUE;
      *((cl_layer_api_version *)param_value) = CL_LAYER_API_VERSION_100;
    }
    if (param_value_size_ret)
      *param_value_size_ret = sizeof(cl_layer_api_version);
    break;
  case CL_LAYER_NAME: {
    const char *LayerName = "CLVizulayer";
    size_t Bytes = strlen(LayerName) + 1;

    char *StrPtr = static_cast<char *>(param_value);
    if (StrPtr != nullptr) {
      if (param_value_size < Bytes) {
        return CL_INVALID_VALUE;
      }
      strcpy(StrPtr, LayerName);
    }
    if (param_value_size_ret != nullptr) {
      *param_value_size_ret = Bytes;
    }
    break;
  }
  default:
    return CL_INVALID_VALUE;
  }
  return CL_SUCCESS;
}

static struct _cl_icd_dispatch ShimDispatch;
const struct _cl_icd_dispatch *TargetDispatch;

static void _init_dispatch();

CL_API_ENTRY cl_int CL_API_CALL
clInitLayer(cl_uint num_entries, const struct _cl_icd_dispatch *target_dispatch,
            cl_uint *num_entries_out,
            const struct _cl_icd_dispatch **layer_dispatch_ret) {
  // Spec error case:
  // CL_INVALID_VALUE if target_dispatch is a NULL value, or num_entries_ret is
  // a NULL value, or layer_dispatch_ret is a NULL value.
  if (!target_dispatch || !num_entries_out || !layer_dispatch_ret) {
    return CL_INVALID_VALUE;
  }

  /* Check that the loader does not provide us with a dispatch table
   * smaller than the one we've been compiled with. */
  constexpr size_t ShimLayerSize =
      sizeof(ShimDispatch) / sizeof(ShimDispatch.clEnqueueNDRangeKernel);
  if (num_entries < ShimLayerSize) {
    return CL_INVALID_VALUE;
  }

  TargetDispatch = target_dispatch;
  _init_dispatch();
  *layer_dispatch_ret = &ShimDispatch;
  *num_entries_out = ShimLayerSize;
  return CL_SUCCESS;
}

cl_int clEnqueueNDRangeKernelShim(
    cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim,
    const size_t *global_work_offset, const size_t *global_work_size,
    const size_t *local_work_size, cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list, cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueNDRangeKernel(
      command_queue, kernel, work_dim, global_work_offset, global_work_size,
      local_work_size, num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    std::function<void(VizDotFile *)> VerbosePrint{};
    auto &Context = getVizContext();
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\ncl_kernel = " << std::hex << kernel << std::dec;
          Stream << "\\n";

          size_t Size;
          cl_int Ret = TargetDispatch->clGetKernelInfo(
              kernel, CL_KERNEL_FUNCTION_NAME, 0, nullptr, &Size);

          if (Ret != CL_SUCCESS) {
            VIZ_ERR("Error calling clGetKernelInfo, err code {}", Ret);
            return;
          }

          std::vector<char> FuncName(Size, '\0');
          Ret = TargetDispatch->clGetKernelInfo(kernel, CL_KERNEL_FUNCTION_NAME,
                                                Size, FuncName.data(), nullptr);
          if (Ret != CL_SUCCESS) {
            VIZ_ERR("Error calling clGetKernelInfo, err code {}", Ret);
            return;
          }
          Stream << "\nname = " << FuncName.data();
        };
      }
      Context.createVizNode(
          command_queue, "clEnqueueNDRangeKernel", std::move(VerbosePrint),
          std::span(event_wait_list, num_events_in_wait_list), event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating kernel node: {}", E.what());
    }
  }

  return Ret;
}

cl_int clEnqueueTaskShim(cl_command_queue command_queue, cl_kernel kernel,
                         cl_uint num_events_in_wait_list,
                         const cl_event *event_wait_list, cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueTask(
      command_queue, kernel, num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    std::function<void(VizDotFile *)> VerbosePrint{};
    auto &Context = getVizContext();
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\ncl_kernel = " << std::hex << kernel << std::dec;
          Stream << "\\n";

          size_t Size;
          cl_int Ret = TargetDispatch->clGetKernelInfo(
              kernel, CL_KERNEL_FUNCTION_NAME, 0, nullptr, &Size);

          if (Ret != CL_SUCCESS) {
            VIZ_ERR("Error calling clGetKernelInfo, err code {}", Ret);
            return;
          }

          std::vector<char> FuncName(Size, '\0');
          Ret = TargetDispatch->clGetKernelInfo(kernel, CL_KERNEL_FUNCTION_NAME,
                                                Size, FuncName.data(), nullptr);
          if (Ret != CL_SUCCESS) {
            VIZ_ERR("Error calling clGetKernelInfo, err code {}", Ret);
            return;
          }
          Stream << "\nname = " << FuncName.data();
        };
      }
      Context.createVizNode(command_queue, "clEnqueueTask", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating kernel node: {}", E.what());
    }
  }

  return Ret;
}

cl_int clEnqueueNativeKernelShim(
    cl_command_queue command_queue, void(CL_CALLBACK *user_func)(void *),
    void *args, size_t cb_args, cl_uint num_mem_objects, const cl_mem *mem_list,
    const void **args_mem_loc, cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list, cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueNativeKernel(
      command_queue, user_func, args, cb_args, num_mem_objects, mem_list,
      args_mem_loc, num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    try {
      Context.createVizNode(command_queue, "clEnqueueNativeKernel", {},
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating native node: {}", E.what());
    }
  }

  return Ret;
}

cl_command_queue
clCreateCommandQueueShim(cl_context Context, cl_device_id device,
                         cl_command_queue_properties properties,
                         cl_int *errcode_ret) {
  cl_command_queue CQ = TargetDispatch->clCreateCommandQueue(
      Context, device, properties, errcode_ret);
  if (CQ != nullptr) {
    auto &Context = getVizContext();
    const bool IsInOrder =
        (properties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) == 0;
    try {
      Context.createVizQueue(CQ, IsInOrder);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating VizQueue: {}", E.what());
    }
  }
  return CQ;
}

cl_command_queue
clCreateCommandQueueWithPropertiesShim(cl_context Context, cl_device_id device,
                                       const cl_queue_properties *properties,
                                       cl_int *errcode_ret) {

  cl_command_queue CQ = TargetDispatch->clCreateCommandQueueWithProperties(
      Context, device, properties, errcode_ret);
  if (CQ != nullptr) {
    auto &Context = getVizContext();

    bool IsInOrder = true;
    if (properties) {
      while (*properties != 0) {
        if (*properties == CL_QUEUE_PROPERTIES) {
          cl_command_queue_properties queue_props = *(properties + 1);
          IsInOrder =
              (queue_props & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) == 0;
          break;
        }
        properties += 2;
      }
    }
    try {
      Context.createVizQueue(CQ, IsInOrder);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating VizQueue: {}", E.what());
    }
  }
  return CQ;
}

cl_int clFinishShim(cl_command_queue command_queue) {
  cl_int Ret = TargetDispatch->clFinish(command_queue);
  auto &Context = getVizContext();
  try {
    Context.flushQueue(command_queue, "clFinish()", false);
  } catch (std::exception &E) {
    VIZ_ERR("Error flushing VizQueue: {}", E.what());
  }
  return Ret;
}

cl_int clReleaseCommandQueueShim(cl_command_queue command_queue) {
  cl_int Ret = TargetDispatch->clReleaseCommandQueue(command_queue);
  auto &Context = getVizContext();
  try {
    Context.flushQueue(command_queue, "clReleaseCommandQueue()", false);
  } catch (std::exception &E) {
    VIZ_ERR("Error flushing VizQueue: {}", E.what());
  }
  return Ret;
}

cl_int clEnqueueBarrierShim(cl_command_queue command_queue) {
  cl_int Ret = TargetDispatch->clEnqueueBarrier(command_queue);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    try {
      Context.createVizBarrierNode(command_queue, false,
                                   std::span<const cl_event>(), nullptr);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating barrier node: {}", E.what());
    }
  }

  return Ret;
}

cl_int clEnqueueBarrierWithWaitListShim(cl_command_queue command_queue,
                                        cl_uint num_events_in_wait_list,
                                        const cl_event *event_wait_list,
                                        cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueBarrierWithWaitList(
      command_queue, num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    try {
      Context.createVizBarrierNode(
          command_queue, true,
          std::span(event_wait_list, num_events_in_wait_list), event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating barrier node: {}", E.what());
    }
  }

  return Ret;
}

cl_int clEnqueueWaitForEventsShim(cl_command_queue command_queue,
                                  cl_uint num_events,
                                  const cl_event *event_list) {
  cl_int Ret = TargetDispatch->clEnqueueWaitForEvents(command_queue, num_events,
                                                      event_list);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    try {
      Context.createVizNode(command_queue, "clEnqueueWaitForEvents", {},
                            std::span(event_list, num_events), nullptr);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating wait node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueMarkerShim(cl_command_queue command_queue, cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueMarker(command_queue, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    try {
      Context.createVizMarkerNode(command_queue, event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating marker node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueMarkerWithWaitListShim(cl_command_queue command_queue,
                                       cl_uint num_events_in_wait_list,
                                       const cl_event *event_wait_list,
                                       cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueMarkerWithWaitList(
      command_queue, num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    try {
      Context.createVizMarkerNode(
          command_queue, std::span(event_wait_list, num_events_in_wait_list),
          event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating marker node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueFillBufferShim(cl_command_queue command_queue, cl_mem buffer,
                               const void *pattern, size_t pattern_size,
                               size_t offset, size_t size,
                               cl_uint num_events_in_wait_list,
                               const cl_event *event_wait_list,
                               cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueFillBuffer(
      command_queue, buffer, pattern, pattern_size, offset, size,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\ncl_mem = " << std::hex << buffer;
          Stream << "\\n";
          Stream << "\npattern = " << pattern << std::dec;
          Stream << "\\n";
          Stream << "\npattern_size = " << pattern_size;
          Stream << "\\n";
          Stream << "\noffset = " << offset;
          Stream << "\\n";
          Stream << "\nsize = " << size;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueFillBuffer", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);

    } catch (std::exception &E) {
      VIZ_ERR("Error creating buffer fill node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueCopyBufferShim(cl_command_queue command_queue,
                               cl_mem src_buffer, cl_mem dst_buffer,
                               size_t src_offset, size_t dst_offset,
                               size_t size, cl_uint num_events_in_wait_list,
                               const cl_event *event_wait_list,
                               cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueCopyBuffer(
      command_queue, src_buffer, dst_buffer, src_offset, dst_offset, size,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nsrc_buffer = " << std::hex << src_buffer;
          Stream << "\\n";
          Stream << "\ndst_buffer = " << dst_buffer << std::dec;
          Stream << "\\n";
          Stream << "\nsrc_offset = " << src_offset;
          Stream << "\\n";
          Stream << "\ndst_offset = " << dst_offset;
          Stream << "\\n";
          Stream << "\nsize = " << size;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueCopyBuffer", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating buffer copy node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueReadBufferShim(cl_command_queue command_queue, cl_mem buffer,
                               cl_bool blocking_read, size_t offset,
                               size_t size, void *ptr,
                               cl_uint num_events_in_wait_list,
                               const cl_event *event_wait_list,
                               cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueReadBuffer(
      command_queue, buffer, blocking_read, offset, size, ptr,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    std::function<void(VizDotFile *)> VerbosePrint{};
    auto &Context = getVizContext();
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nblocking = " << (blocking_read ? "True" : "False");
          Stream << "\\n";
          Stream << "\nbuffer = " << std::hex << buffer << std::dec;
          Stream << "\\n";
          Stream << "\noffset = " << offset;
          Stream << "\\n";
          Stream << "\nsize = " << size;
          Stream << "\\n";
          Stream << "\nptr = " << std::hex << ptr << std::dec;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueReadBuffer", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);

      if (blocking_read) {
        Context.flushQueue(command_queue, "clEnqueueReadBuffer()", true);
      }
    } catch (std::exception &E) {
      VIZ_ERR("Error creating buffer read node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueWriteBufferShim(cl_command_queue command_queue, cl_mem buffer,
                                cl_bool blocking_write, size_t offset,
                                size_t size, const void *ptr,
                                cl_uint num_events_in_wait_list,
                                const cl_event *event_wait_list,
                                cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueWriteBuffer(
      command_queue, buffer, blocking_write, offset, size, ptr,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nblocking = " << (blocking_write ? "True" : "False");
          Stream << "\\n";
          Stream << "\nbuffer = " << std::hex << buffer << std::dec;
          Stream << "\\n";
          Stream << "\noffset = " << offset;
          Stream << "\\n";
          Stream << "\nsize = " << size;
          Stream << "\\n";
          Stream << "\nptr = " << std::hex << ptr << std::dec;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueWriteBuffer", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);

      if (blocking_write) {
        Context.flushQueue(command_queue, "clEnqueueWriteBuffer()", true);
      }
    } catch (std::exception &E) {
      VIZ_ERR("Error creating buffer write node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueCopyBufferRectShim(
    cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_buffer,
    const size_t *src_origin, const size_t *dst_origin, const size_t *region,
    size_t src_row_pitch, size_t src_slice_pitch, size_t dst_row_pitch,
    size_t dst_slice_pitch, cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list, cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueCopyBufferRect(
      command_queue, src_buffer, dst_buffer, src_origin, dst_origin, region,
      src_row_pitch, src_slice_pitch, dst_row_pitch, dst_slice_pitch,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\\n";
          Stream << "\nsrc_buffer = " << std::hex << src_buffer;
          Stream << "\\n";
          Stream << "\ndst_buffer = " << dst_buffer << std::dec;
          Stream << "\\n";
          Stream << "\nsrc_origin = {" << src_origin[0] << ", ";
          Stream << src_origin[1] << ", " << src_origin[2] << "}";
          Stream << "\\n";
          Stream << "\ndst_origin = {" << dst_origin[0] << ", ";
          Stream << dst_origin[1] << ", " << dst_origin[2] << "}";
          Stream << "\\n";
          Stream << "\nregion = {" << region[0] << ", ";
          Stream << region[1] << ", " << region[2] << "}";
          Stream << "\\n";
          Stream << "\nsrc_row_pitch = " << src_row_pitch;
          Stream << "\\n";
          Stream << "\nsrc_slice_pitch = " << src_slice_pitch;
          Stream << "\\n";
          Stream << "\ndst_row_pitch = " << dst_row_pitch;
          Stream << "\\n";
          Stream << "\ndst_slice_pitch = " << dst_slice_pitch;
        };
      }

      Context.createVizNode(
          command_queue, "clEnqueueCopyBufferRect", VerbosePrint,
          std::span(event_wait_list, num_events_in_wait_list), event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating copy buffer rect node: {}", E.what());
    }
  }
  return Ret;
}

cl_int
clEnqueueReadBufferRectShim(cl_command_queue command_queue, cl_mem buffer,
                            cl_bool blocking_read, const size_t *buffer_origin,
                            const size_t *host_origin, const size_t *region,
                            size_t buffer_row_pitch, size_t buffer_slice_pitch,
                            size_t host_row_pitch, size_t host_slice_pitch,
                            void *ptr, cl_uint num_events_in_wait_list,
                            const cl_event *event_wait_list, cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueReadBufferRect(
      command_queue, buffer, blocking_read, buffer_origin, host_origin, region,
      buffer_row_pitch, buffer_slice_pitch, host_row_pitch, host_slice_pitch,
      ptr, num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nblocking = " << (blocking_read ? "True" : "False");
          Stream << "\\n";
          Stream << "\nbuffer = " << std::hex << buffer << std::dec;
          Stream << "\\n";
          Stream << "\nbuffer_origin = {" << buffer_origin[0] << ", ";
          Stream << buffer_origin[1] << ", " << buffer_origin[2] << "}";
          Stream << "\\n";
          Stream << "\nhost_origin = {" << host_origin[0] << ", ";
          Stream << host_origin[1] << ", " << host_origin[2] << "}";
          Stream << "\\n";
          Stream << "\nregion = {" << region[0] << ", ";
          Stream << region[1] << ", " << region[2] << "}";
          Stream << "\\n";
          Stream << "\nbuffer_row_pitch = " << buffer_row_pitch;
          Stream << "\\n";
          Stream << "\nbuffer_slice_pitch = " << buffer_slice_pitch;
          Stream << "\\n";
          Stream << "\nhost_row_pitch = " << host_row_pitch;
          Stream << "\\n";
          Stream << "\nhost_slice_pitch = " << host_slice_pitch;
          Stream << "\\n";
          Stream << "\nptr = " << std::hex << ptr << std::dec;
        };
      }

      Context.createVizNode(
          command_queue, "clEnqueueReadBufferRect", VerbosePrint,
          std::span(event_wait_list, num_events_in_wait_list), event);
      if (blocking_read) {
        Context.flushQueue(command_queue, "clEnqueueReadBufferRect()", true);
      }
    } catch (std::exception &E) {
      VIZ_ERR("Error creating read buffer rect node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueWriteBufferRectShim(
    cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write,
    const size_t *buffer_origin, const size_t *host_origin,
    const size_t *region, size_t buffer_row_pitch, size_t buffer_slice_pitch,
    size_t host_row_pitch, size_t host_slice_pitch, const void *ptr,
    cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
    cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueWriteBufferRect(
      command_queue, buffer, blocking_write, buffer_origin, host_origin, region,
      buffer_row_pitch, buffer_slice_pitch, host_row_pitch, host_slice_pitch,
      ptr, num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nblocking = " << (blocking_write ? "True" : "False");
          Stream << "\\n";
          Stream << "\nbuffer = " << std::hex << buffer << std::dec;
          Stream << "\\n";
          Stream << "\nbuffer_origin = {" << buffer_origin[0] << ", ";
          Stream << buffer_origin[1] << ", " << buffer_origin[2] << "}";
          Stream << "\\n";
          Stream << "\nhost_origin = {" << host_origin[0] << ", ";
          Stream << host_origin[1] << ", " << host_origin[2] << "}";
          Stream << "\\n";
          Stream << "\nregion = {" << region[0] << ", ";
          Stream << region[1] << ", " << region[2] << "}";
          Stream << "\\n";
          Stream << "\nbuffer_row_pitch = " << buffer_row_pitch;
          Stream << "\\n";
          Stream << "\nbuffer_slice_pitch = " << buffer_slice_pitch;
          Stream << "\\n";
          Stream << "\nhost_row_pitch = " << host_row_pitch;
          Stream << "\\n";
          Stream << "\nhost_slice_pitch = " << host_slice_pitch;
          Stream << "\\n";
          Stream << "\nptr = " << std::hex << ptr << std::dec;
        };
      }

      Context.createVizNode(
          command_queue, "clEnqueueWriteBufferRect", VerbosePrint,
          std::span(event_wait_list, num_events_in_wait_list), event);
      if (blocking_write) {
        Context.flushQueue(command_queue, "clEnqueueWriteBufferRect()", true);
      }
    } catch (std::exception &E) {
      VIZ_ERR("Error creating write buffer rect node: {}", E.what());
    }
  }
  return Ret;
}

void *clEnqueueMapBufferShim(cl_command_queue command_queue, cl_mem buffer,
                             cl_bool blocking_map, cl_map_flags map_flags,
                             size_t offset, size_t size,
                             cl_uint num_events_in_wait_list,
                             const cl_event *event_wait_list, cl_event *event,
                             cl_int *errcode_ret) {
  cl_int Ret;
  auto Ptr = TargetDispatch->clEnqueueMapBuffer(
      command_queue, buffer, blocking_map, map_flags, offset, size,
      num_events_in_wait_list, event_wait_list, event, &Ret);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nblocking = " << (blocking_map ? "True" : "False");
          Stream << "\\n";
          Stream << "\nbuffer = " << std::hex << buffer << std::dec;
          Stream << "\\n";
          Stream << "\nmap_flags = " << map_flags;
          Stream << "\\n";
          Stream << "\noffset = " << offset;
          Stream << "\\n";
          Stream << "\nsize = " << size;
          Stream << "\\n";
          Stream << "\nmapped_ptr = " << std::hex << Ptr << std::dec;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueMapBuffer", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
      if (blocking_map) {
        Context.flushQueue(command_queue, "clEnqueueMapBuffer()", true);
      }
    } catch (std::exception &E) {
      VIZ_ERR("Error creating buffer map node: {}", E.what());
    }
  }
  if (errcode_ret) {
    *errcode_ret = Ret;
  }
  return Ptr;
}

cl_int clEnqueueFillImageShim(cl_command_queue command_queue, cl_mem image,
                              const void *fill_color, const size_t *origin,
                              const size_t *region,
                              cl_uint num_events_in_wait_list,
                              const cl_event *event_wait_list,
                              cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueFillImage(
      command_queue, image, fill_color, origin, region, num_events_in_wait_list,
      event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\ncl_mem = " << std::hex << image << std::dec;
          Stream << "\\n";
          Stream << "\norigin = {" << origin[0] << ", ";
          Stream << origin[1] << ", " << origin[2] << "}";
          Stream << "\\n";
          Stream << "\nregion = {" << region[0] << ", ";
          Stream << region[1] << ", " << region[2] << "}";
          Stream << "\\n";
          Stream << "\ncolor ptr = " << std::hex << fill_color << std::dec;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueFillImage", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating image fill node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueCopyImageShim(cl_command_queue command_queue, cl_mem src_image,
                              cl_mem dst_image, const size_t *src_origin,
                              const size_t *dst_origin, const size_t *region,
                              cl_uint num_events_in_wait_list,
                              const cl_event *event_wait_list,
                              cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueCopyImage(
      command_queue, src_image, dst_image, src_origin, dst_origin, region,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nsrc_image = " << std::hex << src_image;
          Stream << "\\n";
          Stream << "\ndst_image = " << dst_image << std::dec;
          Stream << "\\n";
          Stream << "\nsrc_origin = {" << src_origin[0] << ", ";
          Stream << src_origin[1] << ", " << src_origin[2] << "}";
          Stream << "\\n";
          Stream << "\ndst_origin = {" << dst_origin[0] << ", ";
          Stream << dst_origin[1] << ", " << dst_origin[2] << "}";
          Stream << "\\n";
          Stream << "\nregion = {" << region[0] << ", ";
          Stream << region[1] << ", " << region[2] << "}";
        };
      }

      Context.createVizNode(command_queue, "clEnqueueCopyImage", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating image copy node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueCopyImageToBufferShim(cl_command_queue command_queue,
                                      cl_mem src_image, cl_mem dst_buffer,
                                      const size_t *src_origin,
                                      const size_t *region, size_t dst_offset,
                                      cl_uint num_events_in_wait_list,
                                      const cl_event *event_wait_list,
                                      cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueCopyImageToBuffer(
      command_queue, src_image, dst_buffer, src_origin, region, dst_offset,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nsrc_image = " << std::hex << src_image;
          Stream << "\\n";
          Stream << "\ndst_buffer = " << dst_buffer << std::dec;
          Stream << "\\n";
          Stream << "\nsrc_origin = {" << src_origin[0] << ", ";
          Stream << src_origin[1] << ", " << src_origin[2] << "}";
          Stream << "\\n";
          Stream << "\nregion = {" << region[0] << ", ";
          Stream << region[1] << ", " << region[2] << "}";
          Stream << "\\n";
          Stream << "\ndst_offset = " << dst_offset;
        };
      }

      Context.createVizNode(
          command_queue, "clEnqueueCopyImageToBuffer", VerbosePrint,
          std::span(event_wait_list, num_events_in_wait_list), event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating image to buffer copy node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueCopyBufferToImageShim(
    cl_command_queue command_queue, cl_mem src_buffer, cl_mem dst_image,
    size_t src_offset, const size_t *dst_origin, const size_t *region,
    cl_uint num_events_in_wait_list, const cl_event *event_wait_list,
    cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueCopyBufferToImage(
      command_queue, src_buffer, dst_image, src_offset, dst_origin, region,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nsrc_buffer = " << std::hex << src_buffer;
          Stream << "\\n";
          Stream << "\ndst_image = " << dst_image << std::dec;
          Stream << "\\n";
          Stream << "\nsrc_offset = " << src_offset;
          Stream << "\\n";
          Stream << "\ndst_origin = {" << dst_origin[0] << ", ";
          Stream << dst_origin[1] << ", " << dst_origin[2] << "}";
          Stream << "\\n";
          Stream << "\nregion = {" << region[0] << ", ";
          Stream << region[1] << ", " << region[2] << "}";
        };
      }

      Context.createVizNode(
          command_queue, "clEnqueueCopyBufferToImage", VerbosePrint,
          std::span(event_wait_list, num_events_in_wait_list), event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating copy buffer to image node: {}", E.what());
    }
  }
  return Ret;
}

void *clEnqueueMapImageShim(cl_command_queue command_queue, cl_mem image,
                            cl_bool blocking_map, cl_map_flags map_flags,
                            const size_t *origin, const size_t *region,
                            size_t *image_row_pitch, size_t *image_slice_pitch,
                            cl_uint num_events_in_wait_list,
                            const cl_event *event_wait_list, cl_event *event,
                            cl_int *errcode_ret) {
  cl_int Ret;
  auto Ptr = TargetDispatch->clEnqueueMapImage(
      command_queue, image, blocking_map, map_flags, origin, region,
      image_row_pitch, image_slice_pitch, num_events_in_wait_list,
      event_wait_list, event, &Ret);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nimage = " << std::hex << image << std::dec;
          Stream << "\\n";
          Stream << "\nblocking = " << (blocking_map ? "True" : "False");
          Stream << "\\n";
          Stream << "\nmap_flags = " << map_flags;
          Stream << "\\n";
          Stream << "\norigin = {" << origin[0] << ", ";
          Stream << origin[1] << ", " << origin[2] << "}";
          Stream << "\\n";
          Stream << "\nregion = {" << region[0] << ", ";
          Stream << region[1] << ", " << region[2] << "}";
          Stream << "\\n";
          Stream << "\nimage_row_pitch = " << image_row_pitch;
          Stream << "\\n";
          Stream << "\nimage_slice_pitch = " << image_slice_pitch;
          Stream << "\\n";
          Stream << "\nmapped_ptr = " << std::hex << Ptr << std::dec;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueMapImage", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
      if (blocking_map) {
        Context.flushQueue(command_queue, "clEnqueueMapImage()", true);
      }
    } catch (std::exception &E) {
      VIZ_ERR("Error creating image map node: {}", E.what());
    }
  }
  if (errcode_ret) {
    *errcode_ret = Ret;
  }
  return Ptr;
}

cl_int clEnqueueReadImageShim(cl_command_queue command_queue, cl_mem image,
                              cl_bool blocking_read, const size_t *origin,
                              const size_t *region, size_t row_pitch,
                              size_t slice_pitch, void *ptr,
                              cl_uint num_events_in_wait_list,
                              const cl_event *event_wait_list,
                              cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueReadImage(
      command_queue, image, blocking_read, origin, region, row_pitch,
      slice_pitch, ptr, num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nimage = " << std::hex << image << std::dec;
          Stream << "\\n";
          Stream << "\nblocking = " << (blocking_read ? "True" : "False");
          Stream << "\\n";
          Stream << "\norigin = {" << origin[0] << ", ";
          Stream << origin[1] << ", " << origin[2] << "}";
          Stream << "\\n";
          Stream << "\nregion = {" << region[0] << ", ";
          Stream << region[1] << ", " << region[2] << "}";
          Stream << "\\n";
          Stream << "\nrow_pitch = " << row_pitch;
          Stream << "\\n";
          Stream << "\nslice_pitch = " << slice_pitch;
          Stream << "\\n";
          Stream << "\nptr = " << std::hex << ptr << std::dec;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueReadImage", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
      if (blocking_read) {
        Context.flushQueue(command_queue, "clEnqueueReadImage()", true);
      }
    } catch (std::exception &E) {
      VIZ_ERR("Error creating image read node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueWriteImageShim(cl_command_queue command_queue, cl_mem image,
                               cl_bool blocking_write, const size_t *origin,
                               const size_t *region, size_t input_row_pitch,
                               size_t input_slice_pitch, const void *ptr,
                               cl_uint num_events_in_wait_list,
                               const cl_event *event_wait_list,
                               cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueWriteImage(
      command_queue, image, blocking_write, origin, region, input_row_pitch,
      input_slice_pitch, ptr, num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nimage = " << std::hex << image << std::dec;
          Stream << "\\n";
          Stream << "\nblocking = " << (blocking_write ? "True" : "False");
          Stream << "\\n";
          Stream << "\norigin = {" << origin[0] << ", ";
          Stream << origin[1] << ", " << origin[2] << "}";
          Stream << "\\n";
          Stream << "\nregion = {" << region[0] << ", ";
          Stream << region[1] << ", " << region[2] << "}";
          Stream << "\\n";
          Stream << "\nrow_pitch = " << input_row_pitch;
          Stream << "\\n";
          Stream << "\nslice_pitch = " << input_slice_pitch;
          Stream << "\\n";
          Stream << "\nptr = " << std::hex << ptr << std::dec;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueWriteImage", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
      if (blocking_write) {
        Context.flushQueue(command_queue, "clEnqueueWriteImage()", true);
      }
    } catch (std::exception &E) {
      VIZ_ERR("Error creating image write node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueUnmapMemObjectShim(cl_command_queue command_queue,
                                   cl_mem memobj, void *mapped_ptr,
                                   cl_uint num_events_in_wait_list,
                                   const cl_event *event_wait_list,
                                   cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueUnmapMemObject(
      command_queue, memobj, mapped_ptr, num_events_in_wait_list,
      event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nmemobj = " << std::hex << memobj;
          Stream << "\\n";
          Stream << "\nmapped_ptr = " << mapped_ptr << std::dec;
        };
      }

      Context.createVizNode(
          command_queue, "clEnqueueUnmapMemObject", VerbosePrint,
          std::span(event_wait_list, num_events_in_wait_list), event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating memobj unmap node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueMigrateMemObjectsShim(cl_command_queue command_queue,
                                      cl_uint num_mem_objects,
                                      const cl_mem *mem_objects,
                                      cl_mem_migration_flags flags,
                                      cl_uint num_events_in_wait_list,
                                      const cl_event *event_wait_list,
                                      cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueMigrateMemObjects(
      command_queue, num_mem_objects, mem_objects, flags,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          for (cl_uint i = 0; i < num_mem_objects; i++) {
            Stream << "\nmem_obj = " << std::hex << mem_objects[i] << std::dec;
            Stream << "\\n";
          }
          Stream << "\nflags = " << flags;
        };
      }
      Context.createVizNode(
          command_queue, "clEnqueueMigrateMemObjects", VerbosePrint,
          std::span(event_wait_list, num_events_in_wait_list), event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating migrate memobjs node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueSVMFreeShim(
    cl_command_queue command_queue, cl_uint num_svm_pointers,
    void *svm_pointers[],
    void(CL_CALLBACK *pfn_free_func)(cl_command_queue queue,
                                     cl_uint num_svm_pointers,
                                     void *svm_pointers[], void *user_data),
    void *user_data, cl_uint num_events_in_wait_list,
    const cl_event *event_wait_list, cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueSVMFree(
      command_queue, num_svm_pointers, svm_pointers, pfn_free_func, user_data,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n" << std::hex;
          for (cl_uint i = 0; i < num_svm_pointers; i++) {
            Stream << "\nsvm_ptr = " << svm_pointers[i];
            Stream << "\\n";
          }
          Stream << "\npfn_free_func = "
                 << reinterpret_cast<void *>(pfn_free_func);
          Stream << "\\n";
          Stream << "\nuser_data = " << user_data;
          Stream << std::dec;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueSVMFree", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating SVM free node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueSVMMemcpyShim(cl_command_queue command_queue,
                              cl_bool blocking_copy, void *dst_ptr,
                              const void *src_ptr, size_t size,
                              cl_uint num_events_in_wait_list,
                              const cl_event *event_wait_list,
                              cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueSVMMemcpy(
      command_queue, blocking_copy, dst_ptr, src_ptr, size,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nblocking = " << (blocking_copy ? "True" : "False");
          Stream << "\\n";
          Stream << "\ndst_ptr = " << std::hex << dst_ptr;
          Stream << "\\n";
          Stream << "\nsrc_ptr = " << src_ptr << std::dec;
          Stream << "\\n";
          Stream << "\nsize = " << size;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueSVMMemcpy", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
      if (blocking_copy) {
        Context.flushQueue(command_queue, "clEnqueueSVMMemcpy()", true);
      }
    } catch (std::exception &E) {
      VIZ_ERR("Error creating SVM memcpy node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueSVMMemFillShim(cl_command_queue command_queue, void *svm_ptr,
                               const void *pattern, size_t pattern_size,
                               size_t size, cl_uint num_events_in_wait_list,
                               const cl_event *event_wait_list,
                               cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueSVMMemFill(
      command_queue, svm_ptr, pattern, pattern_size, size,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nsvm_ptr = " << std::hex << svm_ptr;
          Stream << "\\n";
          Stream << "\npattern = " << pattern << std::dec;
          Stream << "\\n";
          Stream << "\npattern_size = " << pattern_size;
          Stream << "\\n";
          Stream << "\nsize = " << size;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueSVMMemFill", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating SVM fill node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueSVMMapShim(cl_command_queue command_queue, cl_bool blocking_map,
                           cl_map_flags flags, void *svm_ptr, size_t size,
                           cl_uint num_events_in_wait_list,
                           const cl_event *event_wait_list, cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueSVMMap(
      command_queue, blocking_map, flags, svm_ptr, size,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nblocking = " << (blocking_map ? "True" : "False");
          Stream << "\\n";
          Stream << "\nsvm_ptr = " << std::hex << svm_ptr << std::dec;
          Stream << "\\n";
          Stream << "\nflags = " << flags;
          Stream << "\\n";
          Stream << "\nsize = " << size;
        };
      }

      Context.createVizNode(command_queue, "clEnqueueSVMMap", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
      if (blocking_map) {
        Context.flushQueue(command_queue, "clEnqueueSVMMap()", true);
      }
    } catch (std::exception &E) {
      VIZ_ERR("Error creating SVM map node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clEnqueueSVMUnmapShim(cl_command_queue command_queue, void *svm_ptr,
                             cl_uint num_events_in_wait_list,
                             const cl_event *event_wait_list, cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueSVMUnmap(
      command_queue, svm_ptr, num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          Stream << "\nsvm_ptr = " << std::hex << svm_ptr << std::dec;
        };
      }
      Context.createVizNode(command_queue, "clEnqueueSVMUnmap", VerbosePrint,
                            std::span(event_wait_list, num_events_in_wait_list),
                            event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating SVM unmap node: {}", E.what());
    }
  }
  return Ret;
}

cl_int
clEnqueueSVMMigrateMemShim(cl_command_queue command_queue,
                           cl_uint num_svm_pointers, const void **svm_pointers,
                           const size_t *sizes, cl_mem_migration_flags flags,
                           cl_uint num_events_in_wait_list,
                           const cl_event *event_wait_list, cl_event *event) {
  cl_int Ret = TargetDispatch->clEnqueueSVMMigrateMem(
      command_queue, num_svm_pointers, svm_pointers, sizes, flags,
      num_events_in_wait_list, event_wait_list, event);
  if (Ret == CL_SUCCESS) {
    auto &Context = getVizContext();
    std::function<void(VizDotFile *)> VerbosePrint{};
    try {
      if (Context.verbose()) {
        VerbosePrint = [=](VizDotFile *DotFile) {
          auto &Stream = DotFile->MDotFile;
          Stream << "\\n";
          for (cl_uint i = 0; i < num_svm_pointers; i++) {
            Stream << "\nsvm_ptr = " << std::hex << svm_pointers[i] << std::dec;
            Stream << "\\n";
            Stream << "\nsize = " << sizes[i];
            Stream << "\\n";
          }
          Stream << "\nflags = " << flags;
        };
      }
      Context.createVizNode(
          command_queue, "clEnqueueSVMMigrateMem", VerbosePrint,
          std::span(event_wait_list, num_events_in_wait_list), event);
    } catch (std::exception &E) {
      VIZ_ERR("Error creating SVM migrate node: {}", E.what());
    }
  }
  return Ret;
}

cl_int clWaitForEventsShim(cl_uint num_events, const cl_event *event_list) {
  cl_int Ret = TargetDispatch->clWaitForEvents(num_events, event_list);
  auto &Context = getVizContext();
  try {
    Context.flushEvents(std::span(event_list, num_events));
  } catch (std::exception &E) {
    VIZ_ERR("Error flushing VizNodes: {}", E.what());
  }
  return Ret;
}

void *clGetExtensionFunctionAddressForPlatformShim(cl_platform_id platform,
                                                   const char *funcname) {
#define GET_EXTENSION_FUNCTION(FUNC)                                           \
  if (0 == strcmp(funcname, #FUNC)) {                                          \
    return (void *)FUNC;                                                       \
  }

  auto &Context = getVizContext();
  if (Context.useExt()) {
    GET_EXTENSION_FUNCTION(clCreateDotGraphEXT);
    GET_EXTENSION_FUNCTION(clReleaseDotGraphEXT);
    GET_EXTENSION_FUNCTION(clRetainDotGraphEXT);
  }

  void *RetPtr = TargetDispatch->clGetExtensionFunctionAddressForPlatform(
      platform, funcname);

  const char *EnqueuePrefix = "clEnqueue";
  if (0 == strncmp(funcname, EnqueuePrefix, strlen(EnqueuePrefix))) {
    VIZ_ERR("Warning {} not tracked in dot graph layer", funcname);
  }
  return RetPtr;
}

cl_int clGetDeviceInfoShim(cl_device_id device, cl_device_info param_name,
                           size_t param_value_size, void *param_value,
                           size_t *param_value_size_ret) {

  if (auto &Context = getVizContext(); !Context.useExt()) {
    return TargetDispatch->clGetDeviceInfo(device, param_name, param_value_size,
                                           param_value, param_value_size_ret);
  }
  switch (param_name) {
  case CL_DEVICE_EXTENSIONS: {
    size_t Size = 0;
    TargetDispatch->clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, 0, nullptr,
                                    &Size);

    std::vector<char> DeviceExtensions(Size);
    TargetDispatch->clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, Size,
                                    DeviceExtensions.data(), nullptr);

    std::string ExtensionStr(DeviceExtensions.data());
    ExtensionStr += (ExtensionStr.back() == ' ') ? "cl_ext_dot_graph "
                                                 : " cl_ext_dot_graph";

    const size_t StrBytes = ExtensionStr.size() + 1;
    if (param_value_size_ret) {
      *param_value_size_ret = StrBytes;
    }
    if (param_value) {
      if (param_value_size < StrBytes) {
        return CL_INVALID_VALUE;
      }
      strcpy((char *)param_value, ExtensionStr.c_str());
    }
    return CL_SUCCESS;
  }
  case CL_DEVICE_EXTENSIONS_WITH_VERSION: {
    size_t Size = 0;
    TargetDispatch->clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS_WITH_VERSION,
                                    0, nullptr, &Size);

    const size_t NumExtensions = Size / sizeof(cl_name_version);
    std::vector<cl_name_version> DeviceExtensions(NumExtensions);
    TargetDispatch->clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS_WITH_VERSION,
                                    Size, DeviceExtensions.data(), nullptr);

    cl_name_version DotGraphExt;

    // Set name
    memset(DotGraphExt.name, 0, CL_NAME_VERSION_MAX_NAME_SIZE);
    strcpy(DotGraphExt.name, "cl_ext_dot_graph");
    // Set version
    DotGraphExt.version = CL_MAKE_VERSION(0, 1, 0);
    DeviceExtensions.push_back(DotGraphExt);

    // Copy back to user
    const size_t Bytes = DeviceExtensions.size() * sizeof(cl_name_version);
    if (param_value_size_ret) {
      *param_value_size_ret = Bytes;
    }
    if (param_value) {
      if (param_value_size < Bytes) {
        return CL_INVALID_VALUE;
      }
      memcpy(param_value, DeviceExtensions.data(), Bytes);
    }

    return CL_SUCCESS;
  }
  default:
    break;
  }
  return TargetDispatch->clGetDeviceInfo(device, param_name, param_value_size,
                                         param_value, param_value_size_ret);
}

cl_int clGetPlatformInfoShim(cl_platform_id platform,
                             cl_platform_info param_name,
                             size_t param_value_size, void *param_value,
                             size_t *param_value_size_ret) {
  if (auto &Context = getVizContext(); !Context.useExt()) {
    return TargetDispatch->clGetPlatformInfo(platform, param_name,
                                             param_value_size, param_value,
                                             param_value_size_ret);
  }

  switch (param_name) {
  case CL_PLATFORM_EXTENSIONS: {
    size_t Size = 0;
    TargetDispatch->clGetPlatformInfo(platform, CL_PLATFORM_EXTENSIONS, 0,
                                      nullptr, &Size);

    std::vector<char> PlatformExtensions(Size);
    TargetDispatch->clGetPlatformInfo(platform, CL_PLATFORM_EXTENSIONS, Size,
                                      PlatformExtensions.data(), nullptr);

    std::string ExtensionStr(PlatformExtensions.data());
    ExtensionStr += (ExtensionStr.back() == ' ') ? "cl_ext_dot_graph "
                                                 : " cl_ext_dot_graph";

    const size_t StrBytes = ExtensionStr.size() + 1;
    if (param_value_size_ret) {
      *param_value_size_ret = StrBytes;
    }
    if (param_value) {
      if (param_value_size < StrBytes) {
        return CL_INVALID_VALUE;
      }
      strcpy((char *)param_value, ExtensionStr.c_str());
    }
    return CL_SUCCESS;
  }
  case CL_PLATFORM_EXTENSIONS_WITH_VERSION: {
    size_t Size = 0;
    TargetDispatch->clGetPlatformInfo(
        platform, CL_PLATFORM_EXTENSIONS_WITH_VERSION, 0, nullptr, &Size);

    const size_t NumExtensions = Size / sizeof(cl_name_version);
    std::vector<cl_name_version> PlatformExtensions(NumExtensions);
    TargetDispatch->clGetPlatformInfo(platform,
                                      CL_PLATFORM_EXTENSIONS_WITH_VERSION, Size,
                                      PlatformExtensions.data(), nullptr);

    cl_name_version DotGraphExt;

    // Set name
    memset(DotGraphExt.name, 0, CL_NAME_VERSION_MAX_NAME_SIZE);
    strcpy(DotGraphExt.name, "cl_ext_dot_graph");
    // Set version
    DotGraphExt.version = CL_MAKE_VERSION(0, 1, 0);
    PlatformExtensions.push_back(DotGraphExt);

    // Copy back to user
    const size_t Bytes = PlatformExtensions.size() * sizeof(cl_name_version);
    if (param_value_size_ret) {
      *param_value_size_ret = Bytes;
    }
    if (param_value) {
      if (param_value_size < Bytes) {
        return CL_INVALID_VALUE;
      }
      memcpy(param_value, PlatformExtensions.data(), Bytes);
    }
    return CL_SUCCESS;
  }
  default:
    break;
  }
  return TargetDispatch->clGetPlatformInfo(platform, param_name,
                                           param_value_size, param_value,
                                           param_value_size_ret);
}

static void _init_dispatch() {
  // Platform and device query entry-points
  ShimDispatch.clGetExtensionFunctionAddressForPlatform =
      &clGetExtensionFunctionAddressForPlatformShim;
  ShimDispatch.clGetDeviceInfo = &clGetDeviceInfoShim;
  ShimDispatch.clGetPlatformInfo = &clGetPlatformInfoShim;

  // command-queue entry-points
  ShimDispatch.clCreateCommandQueue = &clCreateCommandQueueShim;
  ShimDispatch.clCreateCommandQueueWithProperties =
      &clCreateCommandQueueWithPropertiesShim;
  ShimDispatch.clReleaseCommandQueue = &clReleaseCommandQueueShim;
  ShimDispatch.clFinish = &clFinishShim;

  // event entry-points
  ShimDispatch.clWaitForEvents = &clWaitForEventsShim;

  // kernel enqueue entry-points
  ShimDispatch.clEnqueueNDRangeKernel = &clEnqueueNDRangeKernelShim;
  ShimDispatch.clEnqueueTask = &clEnqueueTaskShim;
  ShimDispatch.clEnqueueNativeKernel = &clEnqueueNativeKernelShim;

  // barrier/wait-list enqueue entry-points
  ShimDispatch.clEnqueueBarrier = &clEnqueueBarrierShim;
  ShimDispatch.clEnqueueBarrierWithWaitList = &clEnqueueBarrierWithWaitListShim;
  ShimDispatch.clEnqueueWaitForEvents = &clEnqueueWaitForEventsShim;

  // marker enqueue entry-points
  ShimDispatch.clEnqueueMarker = &clEnqueueMarkerShim;
  ShimDispatch.clEnqueueMarkerWithWaitList = &clEnqueueMarkerWithWaitListShim;

  // buffer enqueue entry-points
  ShimDispatch.clEnqueueFillBuffer = &clEnqueueFillBufferShim;
  ShimDispatch.clEnqueueCopyBuffer = &clEnqueueCopyBufferShim;
  ShimDispatch.clEnqueueReadBuffer = &clEnqueueReadBufferShim;
  ShimDispatch.clEnqueueWriteBuffer = &clEnqueueWriteBufferShim;
  ShimDispatch.clEnqueueCopyBufferRect = &clEnqueueCopyBufferRectShim;
  ShimDispatch.clEnqueueReadBufferRect = &clEnqueueReadBufferRectShim;
  ShimDispatch.clEnqueueWriteBufferRect = &clEnqueueWriteBufferRectShim;
  ShimDispatch.clEnqueueMapBuffer = &clEnqueueMapBufferShim;

  // image enqueue entry-points
  ShimDispatch.clEnqueueFillImage = &clEnqueueFillImageShim;
  ShimDispatch.clEnqueueCopyImage = &clEnqueueCopyImageShim;
  ShimDispatch.clEnqueueCopyImageToBuffer = &clEnqueueCopyImageToBufferShim;
  ShimDispatch.clEnqueueCopyBufferToImage = &clEnqueueCopyBufferToImageShim;
  ShimDispatch.clEnqueueMapImage = &clEnqueueMapImageShim;
  ShimDispatch.clEnqueueReadImage = &clEnqueueReadImageShim;
  ShimDispatch.clEnqueueWriteImage = &clEnqueueWriteImageShim;

  // mem obj enqueue entry-points
  ShimDispatch.clEnqueueUnmapMemObject = &clEnqueueUnmapMemObjectShim;
  ShimDispatch.clEnqueueMigrateMemObjects = &clEnqueueMigrateMemObjectsShim;

  // SVM entry-points
  ShimDispatch.clEnqueueSVMFree = &clEnqueueSVMFreeShim;
  ShimDispatch.clEnqueueSVMMemcpy = &clEnqueueSVMMemcpyShim;
  ShimDispatch.clEnqueueSVMMemFill = &clEnqueueSVMMemFillShim;
  ShimDispatch.clEnqueueSVMMap = &clEnqueueSVMMapShim;
  ShimDispatch.clEnqueueSVMUnmap = &clEnqueueSVMUnmapShim;
  ShimDispatch.clEnqueueSVMMigrateMem = &clEnqueueSVMMigrateMemShim;
}
