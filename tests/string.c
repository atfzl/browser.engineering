#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../src/utils/string.c"

void test_starts_with(void** state) {
    assert_true(starts_with("https://www.example.com", "https://"));
}

int main(int argc, char** argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_starts_with),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
