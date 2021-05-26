#pragma once

typedef struct {
  char *status;
  char *headers;
  char *html;
} http_response_t;

void request(char *url, http_response_t *response);

const char *lex(const char *html);
