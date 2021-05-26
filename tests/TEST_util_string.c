// clang-format off
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
// clang-format on

// NOLINTNEXTLINE(bugprone-suspicious-include)
#include "../src/utils/string.c"
#include "cmocka.h"

void TEST_startsWith() {
  assert_true(str_startsWith("https://www.example.com", "https://"));
  assert_false(str_startsWith("https://www.example.com", "ttps://"));
}

void TEST_slice() {
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
      cmocka_unit_test(TEST_slice),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
