#include <stdio.h>
#include <stdlib.h>

#include "./src/http.h"
#include "./src/utils/debug.h"

int main() {
  debug_init();

  http_createRequest("https://example.org/index.html");

  return EXIT_SUCCESS;
}
