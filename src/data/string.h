#pragma once

#include <stdlib.h>

typedef struct {
  size_t _size;
  size_t length;
  char *data;
} string_t;

string_t *string_init();

void string_push(string_t *v, char c);

void string_concat(string_t *v, const char *s);

void string_destroy(string_t *v);
