#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 2048

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

    assert(strcmp(host, "example.org") == 0);
    assert(strcmp(path, "index.html") == 0);

    struct addrinfo hints;
    struct addrinfo* result;

    // clear memory
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int s = getaddrinfo(host, "http", &hints, &result);

    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    int SocketFD =
        socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (SocketFD == -1) {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    if (connect(SocketFD, result->ai_addr, result->ai_addrlen) == -1) {
        perror("cannot connect to socket");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    char message[] = "GET /index.html HTTP/1.0\r\nHost: example.org\r\n\r\n";

    if (write(SocketFD, message, strlen(message)) != strlen(message)) {
        perror("failed write");
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];
    ssize_t nread = read(SocketFD, buf, BUF_SIZE);
    if (nread == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buf[nread] = '\0';

    close(SocketFD);

    printf("%s", buf);
}

int main() {
    char url[] = "http://example.org/index.html";
    struct request_response_type* response;

    request(url, &response);

    return 0;
}
