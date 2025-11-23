# Copyright (c) 2025 Ewan Crawford

cmake_minimum_required(VERSION 3.27 FATAL_ERROR)
project(CLVizulayer VERSION 2.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project defined CMake options
option(VIZ_DEBUG "Enable CLVizlayer debug logging to stdout" OFF)
option(VIZ_OFFLINE_BUILD "Avoids pulling dependencies from GitHub" OFF)

configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
find_program(DOXYGEN doxygen)
if (DOXYGEN)
    find_program(DOT dot)
    if (DOT)
        message(STATUS "Doxygen target enabled: ${DOXYGEN}")
        add_custom_target(doxygen
            COMMAND ${DOXYGEN} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating doxygen documentation." VERBATIM)
    else()
      message(WARNING "Doxygen Disabled: No dot executable was found in path.")
    endif()
else()
    message(WARNING "Doxygen Disabled: No doxygen executable was found in path.")
endif()

include(FetchContent)
if (VIZ_OFFLINE_BUILD)
    find_package(OpenCLHeaders REQUIRED)
    get_target_property(OpenCLIncludeDir OpenCL::Headers INTERFACE_INCLUDE_DIRECTORIES)
else()
    FetchContent_Declare(OpenCL-Headers
        GIT_REPOSITORY  "https://github.com/KhronosGroup/OpenCL-Headers.git"
        GIT_TAG main)
    FetchContent_MakeAvailable(OpenCL-Headers)
    FetchContent_GetProperties(OpenCL-Headers SOURCE_DIR OpenCLIncludeDir)
endif()
message(STATUS "OpenCL Include Directory: ${OpenCLIncludeDir}")

add_subdirectory(source)
add_subdirectory(test)
