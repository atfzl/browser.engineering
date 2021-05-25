#pragma once
#include <stdlib.h>

typedef struct vector_t vector_t;

vector_t *vector_init();

char *vector_data(const vector_t *v);

size_t vector_len(const vector_t *v);

void vector_push(vector_t *v, const char c);

void vector_concat(vector_t *v, const char *s);

void vector_destroy(vector_t *v);
