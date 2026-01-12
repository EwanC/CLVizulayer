// Copyright (c) 2025-2026 Ewan Crawford

#define CL_ENABLE_BETA_EXTENSIONS

#include "cl_ext_command_buffer_dot_print.h"
#include "cl_ext_dot_print.h"

#include "logger.h"
#include "viz_context.h"

CL_API_ENTRY cl_int CL_API_CALL clDotPrintCommandBufferEXT(
    cl_command_buffer_khr command_buffer,
    const cl_command_buffer_dot_print_properties_ext *properties,
    const char *file_path) {
  cl_command_buffer_dot_print_flags_ext Flags = 0;
  if (properties) {
    const cl_command_buffer_dot_print_properties_ext *CurrProp = properties;
    bool SeenFlags = false;
    while (CurrProp[0] != 0) {
      if (CurrProp[0] != CL_COMMAND_BUFFER_DOT_PRINT_FLAGS_EXT || SeenFlags) {
        return CL_INVALID_VALUE;
      }
      Flags = *((const cl_command_buffer_dot_print_flags_ext *)(CurrProp + 1));
      cl_command_buffer_dot_print_flags_ext ValidBits =
          CL_COMMAND_BUFFER_DOT_PRINT_COLOR_EXT |
          CL_COMMAND_BUFFER_DOT_PRINT_VERBOSE_EXT;
      if (Flags > ValidBits) {
        return CL_INVALID_VALUE;
      }
      SeenFlags = true;
      CurrProp += 2;
    }
  }
  auto &Context = viz::getVizContext();
  try {
    Context.flushCommandBuffer(command_buffer, Flags, file_path);
  } catch (std::exception &E) {
    VIZ_ERR("Error flushing VizCommandBuffer: {}", E.what());
  }

  return CL_SUCCESS;
}
