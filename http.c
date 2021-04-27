#include <assert.h>
#include <stdio.h>
#include <string.h>

struct request_response_type {
    char* status;
    char* headers;
    char* html;
};

int starts_with(char* str, char* prefix) {
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

void trim_start(char** str, char* prefix) {
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

void request(char* url, struct request_response_type** response) {
    assert(starts_with(url, "http://"));
    trim_start(&url, "http://");
    printf("%s\n", url);
}

int main() {
    char url[] = "http://example.org/index.html";
    struct request_response_type* response;

    request(url, &response);

    return 0;
}
