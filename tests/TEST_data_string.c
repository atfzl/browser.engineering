// clang-format off
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
// clang-format on

#include <string.h>

// NOLINTNEXTLINE(bugprone-suspicious-include)
#include "../src/data/string.c"

void TEST_string_push() {
  string_t *v = string_init();

  assert_string_equal(v->data, "");
  assert_int_equal(v->length, 0);
  assert_int_equal(v->_size, 8);

  string_push(v, 'a');

  assert_string_equal(v->data, "a");
  assert_int_equal(v->length, 1);
  assert_int_equal(v->_size, 8);

  string_push(v, 'b');
  string_push(v, 'c');
  string_push(v, 'd');
  string_push(v, 'e');
  string_push(v, 'f');
  string_push(v, 'g');

  assert_string_equal(v->data, "abcdefg");
  assert_int_equal(v->length, 7);
  assert_int_equal(v->_size, 8);

  string_push(v, 'h');

  assert_string_equal(v->data, "abcdefgh");
  assert_int_equal(v->length, 8);
  assert_int_equal(v->_size, 16);

  string_destroy(v);
}

void TEST_string_concat() {
  string_t *v = string_init();

  assert_string_equal(v->data, "");
  assert_int_equal(v->length, 0);
  assert_int_equal(v->_size, 8);

  string_concat(v, "abc");

  assert_string_equal(v->data, "abc");
  assert_int_equal(v->length, 3);
  assert_int_equal(v->_size, 8);

  string_concat(v, "defghijklmnopqrst");

  assert_string_equal(v->data, "abcdefghijklmnopqrst");
  assert_int_equal(v->length, 20);
  assert_int_equal(v->_size, 32);

  string_destroy(v);
}

int main() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(TEST_string_push),
      cmocka_unit_test(TEST_string_concat),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
