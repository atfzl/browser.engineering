#include <stdio.h>
#include <stdlib.h>

#include "./src/debug.h"
#include "./src/html/htmlShow.h"
#include "./src/http/http.h"
#include "./src/render/renderHTML.h"

int main() {
  debug_init();

  httpResponse_t *httpResponse = NULL;
  string_t *htmlBody = NULL;

  httpResponse = http_requestHTML("https://example.org/index.html");

  if (!httpResponse) {
    goto fail_http_requestHTML;
  }

  debug("http response html: %s\n", httpResponse->html);

  htmlBody = htmlShow(httpResponse->html);

  renderHTML(htmlBody->data);

  httpResponse_destroy(httpResponse);

  return EXIT_SUCCESS;

fail_http_requestHTML:
  return EXIT_FAILURE;
}
