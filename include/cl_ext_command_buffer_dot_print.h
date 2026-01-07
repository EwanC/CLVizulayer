// Copyright (c) 2025-2026 Ewan Crawford
#ifndef CL_EXT_COMMAND_BUFFER_DOT_PRINT_
#define CL_EXT_COMMAND_BUFFER_DOT_PRINT_

#include <CL/cl_ext.h>

#if defined(CL_ENABLE_BETA_EXTENSIONS)

#define cl_ext_command_buffer_dot_print 1
#define CL_EXT_COMMAND_BUFFER_DOT_PRINT_NAME "cl_ext_command_buffer_dot_print"

#define CL_EXT_COMMAND_BUFFER_DOT_PRINT_EXTENSION_VERSION                      \
  CL_MAKE_VERSION(0, 2, 0)

typedef cl_bitfield cl_command_buffer_dot_print_flags_ext;
typedef cl_properties cl_command_buffer_dot_print_properties_ext;

#define CL_COMMAND_BUFFER_DOT_PRINT_FLAGS_EXT 0xABCD // Needs reserved in XML
#define CL_COMMAND_BUFFER_DOT_PRINT_COLOR_EXT (1 << 0)
#define CL_COMMAND_BUFFER_DOT_PRINT_VERBOSE_EXT (1 << 1)

typedef cl_int CL_API_CALL clDotPrintCommandBufferEXT_t(
    cl_command_buffer_khr command_buffer,
    const cl_command_buffer_dot_print_properties_ext *properties,
    const char *file_path);
typedef clDotPrintCommandBufferEXT_t *clDotPrintCommandBufferEXT_fn;

#if !defined(CL_NO_NON_ICD_DISPATCH_EXTENSION_PROTOTYPES)
extern CL_API_ENTRY cl_int CL_API_CALL clDotPrintCommandBufferEXT(
    cl_command_buffer_khr command_buffer,
    const cl_command_buffer_dot_print_properties_ext *properties,
    const char *file_path);
#endif /* !defined(CL_NO_NON_ICD_DISPATCH_EXTENSION_PROTOTYPES) */
#endif // CL_ENABLE_BETA_EXTENSIONS
#endif // CL_EXT_COMMAND_BUFFER_DOT_PRINT_
