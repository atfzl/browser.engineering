#include "./httpSSL.h"
#include "../debug.h"
#include <openssl/err.h>

httpSSL_t *httpSSL_init(int socketFD) {
  httpSSL_t *httpSSL = NULL;
  const SSL_METHOD *method = NULL;
  int status = -1;

  httpSSL = malloc(sizeof(httpSSL_t));

  debug("%s\n", "Trying get SSL");
  method = TLS_client_method(); /* Create new client-method instance */
  httpSSL->sslContext = SSL_CTX_new(method);

  if (httpSSL->sslContext == NULL) {
    ERR_print_errors_fp(stderr);
    goto fail_SSL_CTX_new;
  }

  httpSSL->ssl = SSL_new(httpSSL->sslContext);
  if (httpSSL->ssl == NULL) {
    fprintf(stderr, "SSL_new() failed\n");
    goto fail_SSL_new;
  }

  SSL_set_fd(httpSSL->ssl, socketFD);

  status = SSL_connect(httpSSL->ssl);
  if (status != 1) {
    SSL_get_error(httpSSL->ssl, status);
    ERR_print_errors_fp(stderr); // High probability this doesn't do anything
    fprintf(stderr, "SSL_connect failed with SSL_get_error code %d\n", status);
    goto fail_SSL_connect;
  }

  debug("%s\n", "Got SSL");
  return httpSSL;

fail_SSL_connect:
  SSL_free(httpSSL->ssl);
fail_SSL_new:
  SSL_CTX_free(httpSSL->sslContext);
fail_SSL_CTX_new:
  free(httpSSL);
  return NULL;
}

void httpSSL_destroy(httpSSL_t *httpSSL) {
  SSL_free(httpSSL->ssl);
  SSL_CTX_free(httpSSL->sslContext);
  free(httpSSL);
}
