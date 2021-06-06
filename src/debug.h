#pragma once

#include <stdio.h>
#include <stdlib.h>

void debug_init();

int ENABLE_DEBUG;
#define debug(format, ...)                                                     \
  if (ENABLE_DEBUG)                                                            \
  fprintf(stdout, "DEBUG: " format, __VA_ARGS__)
