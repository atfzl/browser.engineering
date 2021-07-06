#include "./url.h"
#include "../debug.h"
#include "../utils/str.h"
#include <stdlib.h>
#include <string.h>

url_t *url_init(const char *str) {
  int colonIndex = -1;
  int pathSlashIndex = -1;
  url_t *url = NULL;

  colonIndex = str_indexOf(str, ':');

  if (colonIndex == -1) {
    goto fail;
  }

  if (str[colonIndex + 1] != '/' || str[colonIndex + 2] != '/') {
    goto fail;
  }

  pathSlashIndex = str_indexOf(&str[colonIndex + 3], '/');

  if (pathSlashIndex == -1) {
    goto fail;
  }

  // to make the index relative to str again
  pathSlashIndex += colonIndex + 3;

  url = malloc(sizeof(url_t));

  url->scheme = str_slice(str, 0, colonIndex - 1);
  url->host = str_slice(str, colonIndex + 3, pathSlashIndex - 1);
  url->path = str_slice(str, pathSlashIndex, strlen(str) - 1);

  debug("URL Scheme: %s, URL Host: %s, URL Path: %s\n", url->scheme, url->host,
        url->path);
  return url;

fail:
  return NULL;
}

void url_destroy(url_t *url) {
  free(url->scheme);
  free(url->host);
  free(url->path);
  free(url);
};
