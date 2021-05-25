#pragma once
#include <stdlib.h>

typedef struct string_t string_t;

string_t *string_init();

char *string_raw(const string_t *v);

size_t string_len(const string_t *v);

void string_push(string_t *v, char c);

void string_concat(string_t *v, const char *s);

void string_destroy(string_t *v);
