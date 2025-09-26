// Copyright (c) 2025 Ewan Crawford

#pragma once

#include <format>
#include <iostream>

/// @def VIZ_LOG(...)
/// Helper for stout debug logging using printf arguments,
/// only enabled by CMake request
#ifdef VIZ_DEBUG
#define VIZ_LOG(...)                                                           \
  std::cout << "[VIZ LOG] ";                                                   \
  std::cout << std::format(__VA_ARGS__);                                       \
  std::cout << std::endl;
#else
#define VIZ_LOG(...)                                                           \
  do {                                                                         \
  } while (0);
#endif

/// @brief Helper for stderr logging using printf arguments
#define VIZ_ERR(...)                                                           \
  std::cerr << "[VIZ ERROR] ";                                                 \
  std::cerr << std::format(__VA_ARGS__);                                       \
  std::cerr << std::endl;
