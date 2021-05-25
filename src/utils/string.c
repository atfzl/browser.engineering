#include <stdlib.h>
#include <string.h>

int str_startsWith(const char* str, const char* prefix) {
    size_t len = strlen(str);
    size_t prefixLen = strlen(prefix);

    // not possible to have prefix longer than string
    if (len < prefixLen) {
        return 0;
    }

    size_t j = 0;
    while (j < prefixLen && prefix[j] == str[j]) {
        ++j;
    }

    if (j == prefixLen) {
        return 1;
    }

    return 0;
}

void str_trimStart(char** str, const char* prefix) {
    size_t len = strlen(*str);
    size_t prefixLen = strlen(prefix);

    if (len < prefixLen) {
        return;
    }

    size_t j = 0;
    while (j < prefixLen && prefix[j] == (char)**str) {
        ++(*str);
        ++j;
    }
}
