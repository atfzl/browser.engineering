#include "http.h"

#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 2048

int starts_with(char* str, const char* prefix) {
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

void trim_start(char** str, const char* prefix) {
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

void request(char* url, struct request_response_type* response) {
    assert(starts_with(url, "https://"));
    trim_start(&url, "https://");

    // create array of max size because we don't have Vector right now
    char* host = malloc((strlen(url) + 1) * sizeof(char));
    char* path = malloc((strlen(url) + 1) * sizeof(char));

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

    int s = getaddrinfo(host, "https", &hints, &result);

    free(host);
    free(path);

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

    const SSL_METHOD* method =
        TLS_client_method(); /* Create new client-method instance */
    SSL_CTX* ctx = SSL_CTX_new(method);

    if (ctx == NULL) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    SSL* ssl = SSL_new(ctx);
    if (ssl == NULL) {
        fprintf(stderr, "SSL_new() failed\n");
        exit(EXIT_FAILURE);
    }

    SSL_set_fd(ssl, SocketFD);

    const int status = SSL_connect(ssl);
    if (status != 1) {
        SSL_get_error(ssl, status);
        ERR_print_errors_fp(
            stderr);  // High probability this doesn't do anything
        fprintf(stderr, "SSL_connect failed with SSL_get_error code %d\n",
                status);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    char message[] = "GET /index.html HTTP/1.0\r\nHost: example.org\r\n\r\n";

    if (SSL_write(ssl, message, strlen(message)) != strlen(message)) {
        perror("failed write");
        exit(EXIT_FAILURE);
    }

    // TODO dynamically allocate memory as needed
    char total_response[BUF_SIZE * 10];
    total_response[0] = '\0';

    char buf[BUF_SIZE];

    while (1) {
        ssize_t nread = SSL_read(ssl, buf, BUF_SIZE);
        if (nread == 0) {
            break;
        } else if (nread == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        strncat(total_response, buf, nread);
    }

    SSL_free(ssl);
    close(SocketFD);
    SSL_CTX_free(ctx);

    /* +1 during malloc is making space for \0 */
    char* first_newline = strstr(total_response, "\r\n");
    size_t status_len = first_newline - total_response;
    response->status = malloc((status_len + 1) * sizeof(char));
    strncpy(response->status, total_response, status_len);
    (response->status)[status_len] = '\0';

    first_newline += 2;  // skip \r\n

    char* second_newline = strstr(first_newline, "\r\n\r\n");
    size_t headers_len = second_newline - first_newline;
    response->headers = malloc((headers_len + 1) * sizeof(char));
    strncpy(response->headers, first_newline, headers_len);
    (response->headers)[headers_len] = '\0';

    second_newline += 4;  // skip \r\n\r\n

    size_t html_len =
        (total_response + strlen(total_response) - second_newline);
    response->html = malloc((html_len + 1) * sizeof(char));
    strncpy(response->html, second_newline, html_len);
    (response->html)[html_len] = '\0';
}

const char* lex(const char* html) {
    bool in_angle = false;
    size_t l = strlen(html);

    char* text = malloc(9);
    size_t text_max_size = 8;
    size_t text_index = 0;

    for (size_t i = 0; i < l; ++i) {
        char c = html[i];
        if (c == '<') {
            in_angle = true;
        } else if (c == '>') {
            in_angle = false;
        } else if (!in_angle) {
            if (text_index == text_max_size) {
                text_max_size *= 2;
                text = realloc(text, text_max_size + 1);
            }
            text[text_index++] = c;
        }
    }
    text[++text_index] = '\0';
    return text;
}