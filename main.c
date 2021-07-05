#include <stdio.h>
#include <stdlib.h>

#include "./src/debug.h"
#include "./src/html/htmlShow.h"
#include "./src/http/http.h"
#include "./src/render/renderHTML.h"

int main() {
  debug_init();

  httpResponse_t *httpResponse =
      http_requestHTML("https://example.org/index.html");

  if (!httpResponse) {
    return EXIT_FAILURE;
  }

  debug("http response html: %s\n", httpResponse->html);

  string_t *htmlBody = htmlShow(httpResponse->html);

  renderHTML(htmlBody->data);

  httpResponse_destroy(httpResponse);

  return EXIT_SUCCESS;
}
