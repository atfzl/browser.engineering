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

#include "data/string.h"
#include "utils/string.h"
#include "utils/url.h"

#define BUF_SIZE 2048

void request(char *url, http_response_t *response) {
  url_t *urlObject = url_init(url);

  if (!urlObject) {
    exit(EXIT_FAILURE);
  }

  struct addrinfo hints;
  struct addrinfo *result;

  // clear memory
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  int s = getaddrinfo(urlObject->host, "https", &hints, &result);
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

  const SSL_METHOD *method =
      TLS_client_method(); /* Create new client-method instance */
  SSL_CTX *ctx = SSL_CTX_new(method);

  if (ctx == NULL) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  SSL *ssl = SSL_new(ctx);
  if (ssl == NULL) {
    fprintf(stderr, "SSL_new() failed\n");
    exit(EXIT_FAILURE);
  }

  SSL_set_fd(ssl, SocketFD);

  const int status = SSL_connect(ssl);
  if (status != 1) {
    SSL_get_error(ssl, status);
    ERR_print_errors_fp(stderr); // High probability this doesn't do anything
    fprintf(stderr, "SSL_connect failed with SSL_get_error code %d\n", status);
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(result);

  string_t *message = string_init();
  string_concat(message, "GET ");
  string_concat(message, urlObject->path);
  string_concat(message, " HTTP/1.0\r\n");
  string_concat(message, "Host: ");
  string_concat(message, urlObject->host);
  string_concat(message, "\r\n\r\n");

  url_destroy(urlObject);

  char *rawMessage = string_raw(message);

  if ((size_t)SSL_write(ssl, rawMessage, (int)string_len(message)) !=
      string_len(message)) {
    perror("failed write");
    exit(EXIT_FAILURE);
  }

  string_destroy(message);

  string_t *totalResponse = string_init();

  char buf[BUF_SIZE];

  while (1) {
    ssize_t nread = SSL_read(ssl, buf, BUF_SIZE);
    if (nread == 0) {
      break;
    }
    if (nread == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    buf[nread] = '\0';
    string_concat(totalResponse, buf);
  }

  SSL_free(ssl);
  close(SocketFD);
  SSL_CTX_free(ctx);

  const char *rawTotalResponse = string_raw(totalResponse);

  /* +1 during malloc is making space for \0 */
  char *first_newline = strstr(rawTotalResponse, "\r\n");
  size_t status_len = first_newline - rawTotalResponse;
  response->status = malloc((status_len + 1) * sizeof(char));
  strncpy(response->status, rawTotalResponse, status_len);
  (response->status)[status_len] = '\0';

  first_newline += 2; // skip \r\n

  char *second_newline = strstr(first_newline, "\r\n\r\n");
  size_t headers_len = second_newline - first_newline;
  response->headers = malloc((headers_len + 1) * sizeof(char));
  strncpy(response->headers, first_newline, headers_len);
  (response->headers)[headers_len] = '\0';

  second_newline += 4; // skip \r\n\r\n

  size_t html_len =
      (rawTotalResponse + strlen(rawTotalResponse) - second_newline);
  response->html = malloc((html_len + 1) * sizeof(char));
  strncpy(response->html, second_newline, html_len);
  (response->html)[html_len] = '\0';
}

const char *lex(const char *html) {
  bool in_angle = false;
  size_t l = strlen(html);

  string_t *text = string_init();

  for (size_t i = 0; i < l; ++i) {
    char c = html[i];
    if (c == '<') {
      in_angle = true;
    } else if (c == '>') {
      in_angle = false;
    } else if (!in_angle) {
      string_push(text, c);
    }
  }

  return string_raw(text);
}
