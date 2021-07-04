#pragma once

typedef struct {
  char *status;
  char *headers;
  char *html;
} httpResponse_t;

httpResponse_t *httpResponse_init(const char *rawResponseString);

void httpResponse_destroy(httpResponse_t *httpResponse);
