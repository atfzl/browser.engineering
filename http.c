#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
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

    // create array of max size because we don't have Vector right now
    char* host = (char*)malloc(strlen(url) * sizeof(char));
    char* path = (char*)malloc(strlen(url) * sizeof(char));

    {
        size_t hostIndex = 0;
        size_t pathIndex = 0;
        int slash_found = 0;
        for (unsigned int i = 0; i < strlen(url); ++i) {
            char ch = url[i];
            if (ch == '/') {
                slash_found = 1;
                continue;
            }
            if (!slash_found) {
                host[hostIndex++] = ch;
            } else {
                path[pathIndex++] = ch;
            }
        }
        host[hostIndex] = '\0';
        path[pathIndex] = '\0';
    }

    printf("%s %lu\n", host, strlen(host));
    printf("%s %lu\n", path, strlen(path));
}

int main() {
    char url[] = "http://example.org/index.html";
    struct request_response_type* response;

    request(url, &response);

    return 0;
}
