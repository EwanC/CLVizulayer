// Copyright (c) 2025 Ewan Crawford

#define CL_ENABLE_BETA_EXTENSIONS

#include "cl_ext_command_buffer_dot_print.h"
#include "cl_ext_dot_print.h"

#include "logger.h"
#include "viz_context.h"

CL_API_ENTRY cl_int CL_API_CALL clDotPrintCommandBufferEXT(
    cl_command_buffer_khr command_buffer, const char *file_path) {
  auto &Context = getVizContext();
  try {
    Context.flushCommandBuffer(command_buffer, file_path);
  } catch (std::exception &E) {
    VIZ_ERR("Error flushing VizCommandBuffer: {}", E.what());
  }

  return CL_SUCCESS;
}
