#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  size_t _size;
  size_t length;
  char *data;
} vector_t;

#define INITIAL_VECTOR_SIZE 8

vector_t *vector_init() {
  vector_t *v = malloc(sizeof(vector_t));

  v->_size = INITIAL_VECTOR_SIZE;
  v->length = 0;
  v->data = malloc(sizeof(char) * v->_size);
  *(v->data) = '\0'; // a vector will always be null terminated

  return v;
}

char *vector_data(const vector_t *v) { return v->data; }

size_t vector_len(const vector_t *v) { return v->length; }

void _vector_resize(vector_t *v) {
  v->_size = 2 * v->_size;
  v->data = realloc(v->data, sizeof(char) * v->_size);
}

void vector_push(vector_t *v, const char c) {
  if (v->length + 1 >= v->_size) {
    _vector_resize(v);
  }
  (v->data)[v->length] = c;
  ++v->length;
  (v->data)[v->length] = '\0';
}

void vector_concat(vector_t *v, const char *s) {
  size_t sLength = strlen(s);

  while (v->length + sLength >= v->_size) {
    _vector_resize(v);
  }
  strcat(v->data, s);
  (v->length) += sLength;
  (v->data)[v->length] = '\0';
}

void vector_destroy(vector_t *v) {
  free(v->data);
  free(v);
}
