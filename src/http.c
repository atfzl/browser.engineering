#include "./http.h"

http_response_t *make_http_request(const char *urlString) {
  url_t *urlObject = url_init(urlString);

  debug("URL Scheme: %s, URL Host: %s, URL Path: %s\n", urlObject->scheme,
        urlObject->host, urlObject->path);

  if (!urlObject) {
    return NULL;
  }

  return NULL;
}
