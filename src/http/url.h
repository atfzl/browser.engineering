#pragma once

typedef struct {
  char *scheme;
  char *host;
  char *path;
} url_t;

url_t *url_init(const char *str);

void url_destroy(url_t *url);
