// Copyright (c) 2025 Ewan Crawford
#ifndef CL_EXT_DOT_GRAPH_
#define CL_EXT_DOT_GRAPH_

#include <CL/cl.h>

#if defined(CL_ENABLE_BETA_EXTENSIONS)

#define cl_ext_dot_graph 1
#define CL_EXT_DOT_GRAPH_NAME "cl_ext_dot_graph"

#define CL_EXT_DOT_GRAPH_EXTENSION_VERSION CL_MAKE_VERSION(0, 9, 1)

typedef cl_bitfield cl_dot_graph_flags_ext;
typedef cl_properties cl_dot_graph_properties_ext;
typedef struct _cl_dot_graph_ext *cl_dot_graph_ext;

#define CL_DOT_GRAPH_COLOR_EXT (1 << 0)
#define CL_DOT_GRAPH_VERBOSE_EXT (1 << 1)

typedef cl_dot_graph_ext CL_API_CALL
clCreateDotGraphEXT_t(cl_uint num_queues, const cl_command_queue *queues,
                      const cl_dot_graph_properties_ext *properties,
                      const char *file_path, cl_int *errcode_ret);
typedef clCreateDotGraphEXT_t *clCreateDotGraphEXT_fn;

typedef cl_int CL_API_CALL clRetainDotGraphEXT_t(cl_dot_graph_ext dot);
typedef clRetainDotGraphEXT_t *clRetainDotGraphEXT_fn;

typedef cl_int CL_API_CALL clReleaseDotGraphEXT_t(cl_dot_graph_ext dot);
typedef clReleaseDotGraphEXT_t *clReleaseDotGraphEXT_fn;

#if !defined(CL_NO_NON_ICD_DISPATCH_EXTENSION_PROTOTYPES)
extern CL_API_ENTRY cl_dot_graph_ext CL_API_CALL
clCreateDotGraphEXT(cl_uint num_queues, const cl_command_queue *queues,
                    const cl_dot_graph_properties_ext *properties,
                    const char *file_path, cl_int *errcode_ret);

extern CL_API_ENTRY cl_int CL_API_CALL
clRetainDotGraphEXT(cl_dot_graph_ext dot);

extern CL_API_ENTRY cl_int CL_API_CALL
clReleaseDotGraphEXT(cl_dot_graph_ext dot);
#endif /* !defined(CL_NO_NON_ICD_DISPATCH_EXTENSION_PROTOTYPES) */
#endif // CL_ENABLE_BETA_EXTENSIONS
#endif // CL_EXT_DOT_GRAPH_
