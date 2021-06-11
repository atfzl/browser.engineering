#pragma once

typedef struct {
  char *status;
  char *headers;
  char *html;
} http_response_t;

http_response_t *createHTTPRequest(const char *urlString);
