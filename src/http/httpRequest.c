#include "./httpRequest.h"
#include "../debug.h"
#include <stdlib.h>

httpRequest_t *httpRequest_init(const char *urlString) {
  url_t *url = NULL;
  httpRequest_t *request = NULL;

  debug("httpRequest URL String: %s\n", urlString);

  url = url_init(urlString);

  if (!url) {
    goto fail_url_init;
  }

  request = malloc(sizeof(httpRequest_t));
  request->url = url;

  return request;

fail_url_init:
  return NULL;
}

void httpRequest_destroy(httpRequest_t *request) {
  url_destroy(request->url);
  free(request);
}
