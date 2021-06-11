#include "./http.h"
#include "./data/url.h"
#include "./utils/debug.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

struct addrinfo *http_getIPAddressInfo(const char *hostName) {
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
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
  debug("IP v4 address for %s: %s\n", hostName, ipstr);

  return addressInfoResult;
}

static int http_getSocketFD(const struct addrinfo *addressInfo) {
  int socketFD = socket(addressInfo->ai_family, addressInfo->ai_socktype,
                        addressInfo->ai_protocol);

  if (socketFD == -1) {
    perror("cannot create socket");
    return -1;
  }

  if (connect(socketFD, addressInfo->ai_addr, addressInfo->ai_addrlen) == -1) {
    perror("cannot connect to socket");
    return -1;
  }

  return socketFD;
}

static SSL *http_getSSL(int socketFD) {
  const SSL_METHOD *method =
      TLS_client_method(); /* Create new client-method instance */
  SSL_CTX *ctx = SSL_CTX_new(method);

  if (ctx == NULL) {
    ERR_print_errors_fp(stderr);
    return NULL;
  }

  SSL *ssl = SSL_new(ctx);
  if (ssl == NULL) {
    fprintf(stderr, "SSL_new() failed\n");
    return NULL;
  }

  SSL_set_fd(ssl, socketFD);

  const int status = SSL_connect(ssl);
  if (status != 1) {
    SSL_get_error(ssl, status);
    ERR_print_errors_fp(stderr); // High probability this doesn't do anything
    fprintf(stderr, "SSL_connect failed with SSL_get_error code %d\n", status);
    return NULL;
  }

  return ssl;
}

http_response_t *http_createRequest(const char *urlString) {
  url_t *urlObject = url_init(urlString);

  debug("URL String: %s\n", urlString);
  debug("URL Scheme: %s, URL Host: %s, URL Path: %s\n", urlObject->scheme,
        urlObject->host, urlObject->path);

  struct addrinfo *addressInfo = http_getIPAddressInfo(urlObject->host);

  int socketFD = http_getSocketFD(addressInfo);

  if (socketFD == -1) {
    return NULL;
  }

  SSL *ssl = http_getSSL(socketFD);

  if (!ssl) {
    return NULL;
  }

  return NULL;
}
