#pragma once

#include <stdlib.h>

int ENABLE_DEBUG = 0;
void debug_init() { ENABLE_DEBUG = (getenv("DEBUG") != NULL); }
#define debug(format, ...)                                                     \
  if (ENABLE_DEBUG)                                                            \
  fprintf(stdout, "DEBUG: " format, __VA_ARGS__)
