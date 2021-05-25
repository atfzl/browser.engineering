// clang-format off
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
// clang-format on

// NOLINTNEXTLINE(bugprone-suspicious-include)
#include "../src/utils/string.c"

void TEST_startsWith_true() {
  assert_true(str_startsWith("https://www.example.com", "https://"));
}

void TEST_startsWith_false() {
  assert_false(str_startsWith("https://www.example.com", "ttps://"));
}

int main() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(TEST_startsWith_true),
      cmocka_unit_test(TEST_startsWith_false),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
