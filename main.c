#include <stdio.h>
#include <stdlib.h>

#include "./src/debug.h"

int main() {
  debug_init();

  debug("%s\n", "hello world");

  return EXIT_SUCCESS;
}
