#include <stdio.h>
#include <stdlib.h>

#include "src/graphics.h"
#include "src/http.h"

#include "src/utils/debug.h"

int main() {
  init_debug();

  char url[] = "https://example.org/index.html";
  http_response_t response;

  request(url, &response);

  DEBUG_INFO("%s\n", url);

  const char *htmlText = lex(response.html);

  DEBUG_INFO("%s\n", htmlText);

  graphics_main(htmlText);

  free(response.status);
  free(response.headers);
  free(response.html);

  return EXIT_SUCCESS;
}
