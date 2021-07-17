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
  struct addrinfo *addressInfoResult;
  int addressInfoError = -1;
  char ipstr[INET6_ADDRSTRLEN];

  {
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
  }

  addressInfoError = getaddrinfo(hostName, "https", &hints, &addressInfoResult);

  if (addressInfoError != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(addressInfoError));
    goto fail_addrinfo;
  }

  inet_ntop(addressInfoResult->ai_family, addressInfoResult->ai_addr, ipstr,
            sizeof ipstr);
  debug("IPv4 address for %s: %s\n", hostName, ipstr);
  return addressInfoResult;

fail_addrinfo:
  return NULL;
}

static int http_getSocketFD(const struct addrinfo *addressInfo) {
  int socketFD = -1;

  debug("Trying to get socket FD\n");
  debug("before socket()\n");

  socketFD = socket(addressInfo->ai_family, addressInfo->ai_socktype,
                    addressInfo->ai_protocol);

  debug("after socket()\n");

  if (socketFD == -1) {
    perror("cannot create socket");
    goto fail_socket;
  }

  if (connect(socketFD, addressInfo->ai_addr, addressInfo->ai_addrlen) == -1) {
    perror("cannot connect to socket");
    goto fail_connect;
  }

  debug("after socket connect()");
  debug("Got socket FD: %d\n", socketFD);
  return socketFD;

fail_socket:
fail_connect:
  return -1;
}

static string_t *http_createRawMessageRequest(httpRequest_t *request) {
  string_t *message = NULL;

  message = string_init();
  string_concat(message, "GET ");
  string_concat(message, request->url->path);
  string_concat(message, " HTTP/1.0\r\n");
  string_concat(message, "Host: ");
  string_concat(message, request->url->host);
  string_concat(message, "\r\n\r\n");

  return message;
}

static int http_sendRawMessage(SSL *ssl, string_t *message) {
  debug("%s\n", "Sending message");
  if ((size_t)SSL_write(ssl, message->data, (int)(message->length)) !=
      message->length) {
    perror("failed write");
    goto fail_SSL_write;
  }

  debug("%s\n", "Sent message");
  return 0;

fail_SSL_write:
  return -1;
}

static int http_readRawResponse(SSL *ssl, string_t *responseString) {
  char buf[BUF_SIZE];

  debug("Reading response message\n");

  while (1) {
    ssize_t nread = SSL_read(ssl, buf, BUF_SIZE);
    if (nread == 0) {
      break;
    }
    if (nread == -1) {
      perror("read");
      goto fail_SSL_read;
    }
    buf[nread] = '\0';
    string_concat(responseString, buf);
    debug("SSL Read buffer length:\n %s\n", responseString->data);
  }

  debug("Read response message length: %zu\n", responseString->length);
  return 0;

fail_SSL_read:
  return -1;
}

httpResponse_t *http_requestHTML(const char *urlString) {
  httpRequest_t *request = NULL;
  struct addrinfo *addressInfo = NULL;
  int socketFD = -1;
  httpSSL_t *httpSSL = NULL;
  string_t *rawMessage = NULL;
  string_t *responseString = NULL;
  httpResponse_t *httpResponse = NULL;

  request = httpRequest_init(urlString);

  if (!request) {
    goto fail_requestInit;
  }

  addressInfo = http_getIPAddressInfo(request->url->host);

  if (!addressInfo) {
    goto fail_getIPAddressInfo;
  }

  socketFD = http_getSocketFD(addressInfo);

  if (socketFD == -1) {
    goto fail_getSocketFD;
  }

  httpSSL = httpSSL_init(socketFD);

  if (!httpSSL) {
    goto fail_SSL;
  }

  rawMessage = http_createRawMessageRequest(request);

  if (http_sendRawMessage(httpSSL->ssl, rawMessage) == -1) {
    goto fail_sendRawMessage;
  }

  responseString = string_init();

  if (http_readRawResponse(httpSSL->ssl, responseString) == -1) {
    goto fail_readRawResponse;
  }

  httpResponse = httpResponse_init(responseString->data);

  debug("HTTP Response Status: %s\n", httpResponse->status);
  debug("HTTP Response Headers: \n%s\n", httpResponse->headers);

  string_destroy(responseString);
  string_destroy(rawMessage);
  httpSSL_destroy(httpSSL);
  close(socketFD);
  freeaddrinfo(addressInfo);
  httpRequest_destroy(request);
  return httpResponse;

fail_readRawResponse:
  string_destroy(responseString);
fail_sendRawMessage:
  string_destroy(rawMessage);
fail_SSL:
  httpSSL_destroy(httpSSL);
fail_getSocketFD:
  close(socketFD);
fail_getIPAddressInfo:
  freeaddrinfo(addressInfo);
fail_requestInit:
  httpRequest_destroy(request);
  return NULL;
}
