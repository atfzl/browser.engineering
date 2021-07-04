#include "./httpSSL.h"
#include "../utils/debug.h"
#include <openssl/err.h>

httpSSL_t *httpSSL_init(int socketFD) {
  httpSSL_t *httpSSL = malloc(sizeof(httpSSL_t));

  debug("%s\n", "Trying get SSL");
  const SSL_METHOD *method =
      TLS_client_method(); /* Create new client-method instance */
  httpSSL->sslContext = SSL_CTX_new(method);

  if (httpSSL->sslContext == NULL) {
    ERR_print_errors_fp(stderr);
    free(httpSSL);
    return NULL;
  }

  httpSSL->ssl = SSL_new(httpSSL->sslContext);
  if (httpSSL->ssl == NULL) {
    fprintf(stderr, "SSL_new() failed\n");
    free(httpSSL);
    return NULL;
  }

  SSL_set_fd(httpSSL->ssl, socketFD);

  const int status = SSL_connect(httpSSL->ssl);
  if (status != 1) {
    SSL_get_error(httpSSL->ssl, status);
    ERR_print_errors_fp(stderr); // High probability this doesn't do anything
    fprintf(stderr, "SSL_connect failed with SSL_get_error code %d\n", status);
    free(httpSSL);
    return NULL;
  }

  debug("%s\n", "Got SSL");
  return httpSSL;
}

void httpSSL_destroy(httpSSL_t *httpSSL) {
  SSL_CTX_free(httpSSL->sslContext);
  SSL_free(httpSSL->ssl);
  free(httpSSL);
}
