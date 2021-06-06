// clang-format off
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
// clang-format on

#include "../src/utils/str.h"
#include <string.h>

static void TEST_startsWith() {
  assert_true(str_startsWith("https://www.example.com", "https://"));
  assert_false(str_startsWith("https://www.example.com", "ttps://"));
}

static void TEST_indexOf() {
  assert_int_equal(str_indexOf("https://www.example.com", 'c'), 20);
  assert_int_equal(str_indexOf("https://www.example.com", 'h'), 0);
  assert_int_equal(str_indexOf("https://www.example.com", 'm'), 15);
  assert_int_equal(str_indexOf("https://www.example.com", ':'), 5);
  assert_int_equal(str_indexOf("https://www.example.com", 'z'), -1);
}

static void TEST_slice() {
  const char *str = "hello world";

  assert_string_equal(str_slice(str, 0, 0), "h");
  assert_string_equal(str_slice(str, 1, 2), "el");
  assert_string_equal(str_slice(str, 0, 4), "hello");
  assert_string_equal(str_slice(str, 1, 4), "ello");
  assert_string_equal(str_slice(str, 0, 10), "hello world");
  assert_string_equal(str_slice(str, 0, strlen(str) - 1), "hello world");
  assert_null(str_slice(str, -2, 4));
  assert_null(str_slice(str, 0, 11));
}

int main() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(TEST_startsWith),
      cmocka_unit_test(TEST_indexOf),
      cmocka_unit_test(TEST_slice),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
