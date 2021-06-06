#pragma once

#include "./data/url.h"
#include "./debug.h"
#include <stdlib.h>

typedef struct {
  char *status;
  char *headers;
  char *html;
} http_response_t;

http_response_t *make_http_request(const char *urlString);
