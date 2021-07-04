#pragma once

#include <openssl/ssl.h>

typedef struct {
  SSL *ssl;
  SSL_CTX *sslContext;
} httpSSL_t;

httpSSL_t *httpSSL_init();

void httpSSL_destroy(httpSSL_t *httpSSL);
