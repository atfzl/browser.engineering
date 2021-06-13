#include "./http.h"
#include "./data/string.h"
#include "./data/url.h"
#include "./http_request.h"
#include "./utils/debug.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 2048

static struct addrinfo *http_getIPAddressInfo(const char *hostName) {
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET; // IPv4
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  struct addrinfo *addressInfoResult;
  int addressInfoError =
      getaddrinfo(hostName, "https", &hints, &addressInfoResult);
  if (addressInfoError != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(addressInfoError));
    return NULL;
  }

  char ipstr[INET6_ADDRSTRLEN];
  inet_ntop(addressInfoResult->ai_family, addressInfoResult->ai_addr, ipstr,
            sizeof ipstr);
  debug("IPv4 address for %s: %s\n", hostName, ipstr);

  return addressInfoResult;
}

static int http_getSocketFD(const struct addrinfo *addressInfo) {
  debug("before socket()\n");
  int socketFD = socket(addressInfo->ai_family, addressInfo->ai_socktype,
                        addressInfo->ai_protocol);

  debug("after socket()\n");

  if (socketFD == -1) {
    perror("cannot create socket");
    return -1;
  }

  if (connect(socketFD, addressInfo->ai_addr, addressInfo->ai_addrlen) == -1) {
    perror("cannot connect to socket");
    return -1;
  }

  debug("after socket connect()");

  return socketFD;
}

static int http_getSSL(int socketFD, SSL **ssl, SSL_CTX **sslContext) {
  const SSL_METHOD *method =
      TLS_client_method(); /* Create new client-method instance */
  *sslContext = SSL_CTX_new(method);

  if (*sslContext == NULL) {
    ERR_print_errors_fp(stderr);
    return -1;
  }

  *ssl = SSL_new(*sslContext);
  if (ssl == NULL) {
    fprintf(stderr, "SSL_new() failed\n");
    return -1;
  }

  SSL_set_fd(*ssl, socketFD);

  const int status = SSL_connect(*ssl);
  if (status != 1) {
    SSL_get_error(*ssl, status);
    ERR_print_errors_fp(stderr); // High probability this doesn't do anything
    fprintf(stderr, "SSL_connect failed with SSL_get_error code %d\n", status);
    return -1;
  }

  return 0;
}

static string_t *http_createRawMessageRequest(url_t *url) {
  string_t *message = string_init();
  string_concat(message, "GET ");
  string_concat(message, url->path);
  string_concat(message, " HTTP/1.0\r\n");
  string_concat(message, "Host: ");
  string_concat(message, url->host);
  string_concat(message, "\r\n\r\n");

  return message;
}

static void http_parseRawResponse(const char *rawResponseString,
                                  http_response_t **response) {
  *response = malloc(sizeof(http_response_t));

  /* +1 during malloc is making space for \0 */
  char *first_newline = strstr(rawResponseString, "\r\n");
  size_t status_len = first_newline - rawResponseString;
  (*response)->status = malloc((status_len + 1) * sizeof(char));
  strncpy((*response)->status, rawResponseString, status_len);
  ((*response)->status)[status_len] = '\0';

  first_newline += 2; // skip \r\n

  char *second_newline = strstr(first_newline, "\r\n\r\n");
  size_t headers_len = second_newline - first_newline;
  (*response)->headers = malloc((headers_len + 1) * sizeof(char));
  strncpy((*response)->headers, first_newline, headers_len);
  ((*response)->headers)[headers_len] = '\0';

  second_newline += 4; // skip \r\n\r\n

  size_t html_len =
      (rawResponseString + strlen(rawResponseString) - second_newline);
  (*response)->html = malloc((html_len + 1) * sizeof(char));
  strncpy((*response)->html, second_newline, html_len);
  ((*response)->html)[html_len] = '\0';
}

static int http_sendRawMessage(string_t *message, SSL *ssl) {
  if ((size_t)SSL_write(ssl, message->data, (int)(message->length)) !=
      message->length) {
    perror("failed write");
    return -1;
  }

  return 0;
}

static int http_readRawResponse(SSL *ssl, string_t *responseString) {
  char buf[BUF_SIZE];

  while (1) {
    ssize_t nread = SSL_read(ssl, buf, BUF_SIZE);
    if (nread == 0) {
      break;
    }
    if (nread == -1) {
      perror("read");
      return -1;
    }
    buf[nread] = '\0';
    string_concat(responseString, buf);
    debug("SSL Read buffer length:\n %s\n", responseString->data);
  }

  return 0;
}

http_response_t *http_requestHTML(const char *urlString) {
  http_request_t *request = http_request_init(urlString);

  debug("URL String: %s\n", urlString);
  debug("URL Scheme: %s, URL Host: %s, URL Path: %s\n", request->url->scheme,
        request->url->host, request->url->path);

  struct addrinfo *addressInfo = http_getIPAddressInfo(request->url->host);

  debug("Trying to get socket FD\n");
  int socketFD = http_getSocketFD(addressInfo);
  debug("Got socket FD: %d\n", socketFD);

  freeaddrinfo(addressInfo);

  if (socketFD == -1) {
    return NULL;
  }

  SSL *ssl = NULL;
  SSL_CTX *sslContext = NULL;
  debug("%s\n", "Trying get SSL");
  http_getSSL(socketFD, &ssl, &sslContext);
  debug("%s\n", "Got SSL");

  if (!ssl) {
    return NULL;
  }

  string_t *rawMessage = http_createRawMessageRequest(request->url);
  debug("%s\n", "Sending message");
  http_sendRawMessage(rawMessage, ssl);
  debug("%s\n", "Sent message");
  string_destroy(rawMessage);

  string_t *responseString = string_init();
  debug("Reading response message\n");
  if (http_readRawResponse(ssl, responseString) == -1) {
    string_destroy(responseString);
    return NULL;
  }

  debug("Read response message length: %zu\n", responseString->length);

  http_response_t *response;
  http_parseRawResponse(responseString->data, &response);

  debug("HTTP Response Status: %s\n", response->status);
  debug("HTTP Response Headers: \n%s\n", response->headers);

  http_request_destroy(request);
  string_destroy(responseString);
  SSL_CTX_free(sslContext);
  SSL_free(ssl);
  close(socketFD);
  free(response);

  return NULL;
}
