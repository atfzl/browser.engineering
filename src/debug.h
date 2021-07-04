#pragma once

#include <stdio.h>
#include <stdlib.h>

void debug_init();

int ENABLE_DEBUG;
#define debug(...)                                                             \
  if (ENABLE_DEBUG)                                                            \
  fprintf(stdout, "DEBUG: " __VA_ARGS__)
