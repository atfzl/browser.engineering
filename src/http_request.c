#include "./http_request.h"
#include <stdlib.h>

http_request_t *http_request_init(const char *urlString) {
  url_t *url = url_init(urlString);

  http_request_t *request = malloc(sizeof(http_request_t));

  request->url = url;

  return request;
}

void http_request_destroy(http_request_t *request) {
  url_destroy(request->url);
  free(request);
}
