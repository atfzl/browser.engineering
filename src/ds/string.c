#include "./string.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 8

string_t *string_init() {
  string_t *v = malloc(sizeof(string_t));

  v->_size = INITIAL_SIZE;
  v->length = 0;
  v->data = malloc(sizeof(char) * v->_size);
  *(v->data) = '\0'; // a vector will always be null terminated

  return v;
}

static void string_resize(string_t *v) {
  v->_size = 2 * v->_size;
  v->data = realloc(v->data, sizeof(char) * v->_size);
}

void string_push(string_t *v, const char c) {
  if (v->length + 1 >= v->_size) {
    string_resize(v);
  }
  (v->data)[v->length] = c;
  ++v->length;
  (v->data)[v->length] = '\0';
}

void string_concat(string_t *v, const char *s) {
  size_t sLength = strlen(s);

  while (v->length + sLength >= v->_size) {
    string_resize(v);
  }
  strcat(v->data, s);
  (v->length) += sLength;
  (v->data)[v->length] = '\0';
}

void string_destroy(string_t *v) {
  free(v->data);
  free(v);
}
