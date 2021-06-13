#include "./data/url.h"

typedef struct {
  url_t *url;
} http_request_t;

http_request_t *http_request_init(const char *urlString);

void http_request_destroy(http_request_t *request);
