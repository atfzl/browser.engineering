#pragma once

typedef struct {
  char *status;
  char *headers;
  char *html;
} request_response_type;

void request(char *url, request_response_type *response);

const char *lex(const char *html);
