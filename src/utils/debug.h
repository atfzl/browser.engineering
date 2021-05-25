#include <stdlib.h>

int ENABLE_DEBUG = 0;
void init_debug() { ENABLE_DEBUG = (getenv("DEBUG") != NULL); }
#define DEBUG_INFO(format, ...)                                                \
  if (ENABLE_DEBUG)                                                            \
  fprintf(stdout, "DEBUG: " format, __VA_ARGS__)
