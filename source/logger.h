// Copyright (c) 2025 Ewan Crawford

#pragma once

#include <cstdio>

/// @def VIZ_LOG(...)
/// Helper for stout debug logging using printf arguments,
/// only enabled by CMake request
#ifdef VIZ_DEBUG
#define VIZ_LOG(...)                                                           \
  fprintf(stdout, "[VIZ LOG] ");                                               \
  fprintf(stdout, __VA_ARGS__);                                                \
  fprintf(stdout, "\n");                                                       \
  fflush(stdout);
#else
#define VIZ_LOG(...)                                                           \
  do {                                                                         \
  } while (0);
#endif

/// @brief Helper for stderr logging using printf arguments
#define VIZ_ERR(...)                                                           \
  fprintf(stderr, "[VIZ ERROR] ");                                             \
  fprintf(stderr, __VA_ARGS__);                                                \
  fprintf(stderr, "\n");
