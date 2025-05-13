# Copyright (c) 2025 Ewan Crawford

find_program(FileCheck NAMES "filecheck" "FileCheck")
if (NOT FileCheck)
    message(WARNING "Tests Disabled: No `FileCheck` binary was found in path.")
    return()
else()
    message(STATUS "Found ${FileCheck}")
endif()

find_program(LIT lit)
if (NOT LIT)
    message(WARNING "Tests Disabled: No `lit` binary was found in path.")
    return()
else()
    message(STATUS "Found ${LIT}")
endif()

find_package(OpenCLICDLoader)
if(OpenCLICDLoader_FOUND)
    set(OpenCLICDLib OpenCL::OpenCL)
else()
    FetchContent_Declare(OpenCL-ICD-Loader
        GIT_REPOSITORY  "https://github.com/KhronosGroup/OpenCL-ICD-Loader.git"
        GIT_TAG main
    )
    FetchContent_MakeAvailable(OpenCL-ICD-Loader)
    set(OpenCLICDLib OpenCL::OpenCL)
endif()
get_target_property(ICD_LOADER_DIR ${OpenCLICDLib} BINARY_DIR)
message(STATUS "ICD Loader: ${ICD_LOADER_DIR}")

find_package(Python3 REQUIRED COMPONENTS Interpreter)
if (NOT Python3_FOUND)
    message(WARNING "Tests Disabled: No python interpreter was found in path.")
    return()
else()
    message(STATUS "Found ${Python3_EXECUTABLE}")
endif()

find_program(CLInfo clinfo)
if (NOT CLInfo)
    message(WARNING "Tests Disabled: clinfo was not found in path.")
    return()
else()
    message(STATUS "Found ${CLInfo}")
endif()

configure_file(lit.site.cfg.py.in lit.site.cfg.py)
add_custom_target(check
  COMMAND ${Python3_EXECUTABLE} ${LIT} -sv .
  COMMENT "Running LIT test"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  USES_TERMINAL
)
add_dependencies(check CLVizuLayer OpenCL::OpenCL)
