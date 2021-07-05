#include "./http.h"
#include "../data/string.h"
#include "../debug.h"
#include "./httpRequest.h"
#include "./httpResponse.h"
#include "./httpSSL.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
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
  debug("Trying to get socket FD\n");
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
  debug("Got socket FD: %d\n", socketFD);

  return socketFD;
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

static int http_sendRawMessage(string_t *message, SSL *ssl) {
  debug("%s\n", "Sending message");
  if ((size_t)SSL_write(ssl, message->data, (int)(message->length)) !=
      message->length) {
    perror("failed write");
    return -1;
  }

  debug("%s\n", "Sent message");
  return 0;
}

static int http_readRawResponse(SSL *ssl, string_t *responseString) {
  debug("Reading response message\n");

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

  debug("Read response message length: %zu\n", responseString->length);
  return 0;
}

httpResponse_t *http_requestHTML(const char *urlString) {
  httpRequest_t *request = httpRequest_init(urlString);

  if (!request) {
    return NULL;
  }

  struct addrinfo *addressInfo = http_getIPAddressInfo(request->url->host);

  if (!addressInfo) {
    return NULL;
  }

  int socketFD = http_getSocketFD(addressInfo);

  freeaddrinfo(addressInfo);

  if (socketFD == -1) {
    return NULL;
  }

  httpSSL_t *httpSSL = httpSSL_init(socketFD);

  if (!httpSSL) {
    return NULL;
  }

  string_t *rawMessage = http_createRawMessageRequest(request->url);
  http_sendRawMessage(rawMessage, httpSSL->ssl);
  string_destroy(rawMessage);

  string_t *responseString = string_init();

  if (http_readRawResponse(httpSSL->ssl, responseString) == -1) {
    string_destroy(responseString);
    return NULL;
  }

  httpResponse_t *httpResponse = httpResponse_init(responseString->data);
  string_destroy(responseString);

  debug("HTTP Response Status: %s\n", httpResponse->status);
  debug("HTTP Response Headers: \n%s\n", httpResponse->headers);

  httpRequest_destroy(request);
  httpSSL_destroy(httpSSL);
  close(socketFD);

  return httpResponse;
}
