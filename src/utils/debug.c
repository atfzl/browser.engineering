#include "./debug.h"

int ENABLE_DEBUG = 0;
void debug_init() { ENABLE_DEBUG = (getenv("DEBUG") != NULL); }
