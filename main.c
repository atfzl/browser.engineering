#include <stdio.h>
#include <stdlib.h>

#include "./src/http.h"
#include "./src/utils/debug.h"

int main() {
  debug_init();

  httpResponse_t *httpResponse =
      http_requestHTML("https://example.org/index.html");

  printf("http response html: %s\n", httpResponse->html);

  httpResponse_destroy(httpResponse);

  return EXIT_SUCCESS;
}
