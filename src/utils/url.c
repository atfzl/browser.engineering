#include "./url.h"
#include "./string.h"
#include <stdlib.h>
#include <string.h>

url_t *url_init(const char *str) {
  int colonIndex = str_indexOf(str, ':');

  if (colonIndex == -1 || str[colonIndex + 1] != '/' ||
      str[colonIndex + 2] != '/') {
    return NULL;
  }

  int pathSlashIndex = str_indexOf(&str[colonIndex + 3], '/');

  if (pathSlashIndex == -1) {
    return NULL;
  }

  // to make the index relative to str again
  pathSlashIndex += colonIndex + 3;

  url_t *url = malloc(sizeof(url_t));

  char *scheme = str_slice(str, 0, colonIndex - 1);
  char *host = str_slice(str, colonIndex + 3, pathSlashIndex - 1);
  char *path = str_slice(str, pathSlashIndex, strlen(str) - 1);
  url->scheme = scheme;
  url->host = host;
  url->path = path;

  return url;
}

void url_destroy(url_t *url) {
  free(url->scheme);
  free(url->host);
  free(url->path);
  free(url);
};
