# Copyright (c) 2025 Ewan Crawford

if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    add_compile_options(/WX /EHsc)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

set(SOURCES main.cpp viz_context.cpp viz_instance.cpp io.cpp cl_ext_dot_graph.cpp)
if (WIN32)
    list(APPEND SOURCES CLVizulayer.def)
endif()

add_library(CLVizuLayer SHARED ${SOURCES})
target_compile_definitions(CLVizuLayer PRIVATE CL_TARGET_OPENCL_VERSION=300)
target_include_directories(CLVizuLayer PRIVATE ${OpenCLIncludeDir})
target_include_directories(CLVizuLayer PRIVATE ${CMAKE_SOURCE_DIR}/include)
set_target_properties(CLVizuLayer PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(CLVizuLayer PROPERTIES
    VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    SOVERSION "${PROJECT_VERSION_MAJOR}")

if (VIZ_DEBUG)
    target_compile_definitions(CLVizuLayer PRIVATE VIZ_DEBUG)
endif()

include(GNUInstallDirs)
install(TARGETS CLVizuLayer
                ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
                RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES "${CMAKE_SOURCE_DIR}/include/cl_ext_dot_graph.h"
              DESTINATION "include/cl_ext_dot_graph.h")
