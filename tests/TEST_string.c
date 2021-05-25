#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// clang-format off
#include <cmocka.h>
// clang-format on

#include "../src/utils/string.c"

void TEST_startsWith_true(void** state) {
    assert_true(str_startsWith("https://www.example.com", "https://"));
}

void TEST_startsWith_false(void** state) {
    assert_false(str_startsWith("https://www.example.com", "ttps://"));
}

int main(int argc, char** argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(TEST_startsWith_true),
        cmocka_unit_test(TEST_startsWith_false),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
