#include "./httpRequest.h"
#include "../utils/debug.h"
#include <stdlib.h>

httpRequest_t *httpRequest_init(const char *urlString) {
  debug("httpRequest URL String: %s\n", urlString);

  url_t *url = url_init(urlString);

  httpRequest_t *request = malloc(sizeof(httpRequest_t));

  request->url = url;

  return request;
}

void httpRequest_destroy(httpRequest_t *request) {
  url_destroy(request->url);
  free(request);
}
