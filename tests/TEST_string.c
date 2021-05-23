#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../src/utils/string.c"

void TEST_starts_with__true(void** state) {
    assert_true(starts_with("https://www.example.com", "https://"));
}

void TEST_starts_with__false(void** state) {
    assert_false(starts_with("https://www.example.com", "ttps://"));
}

int main(int argc, char** argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(TEST_starts_with__true),
        cmocka_unit_test(TEST_starts_with__false),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
