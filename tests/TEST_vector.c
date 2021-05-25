// clang-format off
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
// clang-format on

#include <string.h>

#include "../src/utils/vector.c"

void TEST_vector_push() {
  vector_t *v = vector_init();

  assert_string_equal(v->data, "");
  assert_int_equal(v->length, 0);
  assert_int_equal(v->_size, 8);

  vector_push(v, 'a');

  assert_string_equal(v->data, "a");
  assert_int_equal(v->length, 1);
  assert_int_equal(v->_size, 8);

  vector_push(v, 'b');
  vector_push(v, 'c');
  vector_push(v, 'd');
  vector_push(v, 'e');
  vector_push(v, 'f');
  vector_push(v, 'g');

  assert_string_equal(v->data, "abcdefg");
  assert_int_equal(v->length, 7);
  assert_int_equal(v->_size, 8);

  vector_push(v, 'h');

  assert_string_equal(v->data, "abcdefgh");
  assert_int_equal(v->length, 8);
  assert_int_equal(v->_size, 16);

  vector_destroy(v);
}

void TEST_vector_concat() {
  vector_t *v = vector_init();

  assert_string_equal(v->data, "");
  assert_int_equal(v->length, 0);
  assert_int_equal(v->_size, 8);

  vector_concat(v, "abc");

  assert_string_equal(v->data, "abc");
  assert_int_equal(v->length, 3);
  assert_int_equal(v->_size, 8);

  vector_concat(v, "defghijklmnopqrst");

  assert_string_equal(v->data, "abcdefghijklmnopqrst");
  assert_int_equal(v->length, 20);
  assert_int_equal(v->_size, 32);

  vector_destroy(v);
}

int main() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(TEST_vector_push),
      cmocka_unit_test(TEST_vector_concat),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
