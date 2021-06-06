// clang-format off
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
// clang-format on

#include "../src/data/url.h"

void TEST_util_url() {
  url_t *url = url_init("https://www.example.com/index.html");
  assert_string_equal(url->scheme, "https");
  assert_string_equal(url->host, "www.example.com");
  assert_string_equal(url->path, "/index.html");
}

int main() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(TEST_util_url),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
