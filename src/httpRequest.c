#include "./httpRequest.h"
#include <stdlib.h>

httpRequest_t *http_request_init(const char *urlString) {
  url_t *url = url_init(urlString);

  httpRequest_t *request = malloc(sizeof(httpRequest_t));

  request->url = url;

  return request;
}

void http_request_destroy(httpRequest_t *request) {
  url_destroy(request->url);
  free(request);
}
