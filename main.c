#include <stdio.h>
#include <stdlib.h>

#include "./src/utils/debug.h"

int main() {
  init_debug();

  debug("%s\n", "hello world");

  return EXIT_SUCCESS;
}
