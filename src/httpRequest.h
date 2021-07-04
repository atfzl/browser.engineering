#pragma once

#include "./data/url.h"

typedef struct {
  url_t *url;
} httpRequest_t;

httpRequest_t *httpRequest_init(const char *urlString);

void httpRequest_destroy(httpRequest_t *request);
