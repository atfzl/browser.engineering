#include "./httpResponse.h"
#include <stdlib.h>
#include <string.h>

httpResponse_t *httpResponse_init(const char *rawResponseString) {
  httpResponse_t *httpResponse = malloc(sizeof(httpResponse_t));

  /* +1 during malloc is making space for \0 */
  char *first_newline = strstr(rawResponseString, "\r\n");
  size_t status_len = first_newline - rawResponseString;
  httpResponse->status = malloc((status_len + 1) * sizeof(char));
  strncpy(httpResponse->status, rawResponseString, status_len);
  (httpResponse->status)[status_len] = '\0';

  first_newline += 2; // skip \r\n

  char *second_newline = strstr(first_newline, "\r\n\r\n");
  size_t headers_len = second_newline - first_newline;
  httpResponse->headers = malloc((headers_len + 1) * sizeof(char));
  strncpy(httpResponse->headers, first_newline, headers_len);
  (httpResponse->headers)[headers_len] = '\0';

  second_newline += 4; // skip \r\n\r\n

  size_t html_len =
      (rawResponseString + strlen(rawResponseString) - second_newline);
  httpResponse->html = malloc((html_len + 1) * sizeof(char));
  strncpy(httpResponse->html, second_newline, html_len);
  (httpResponse->html)[html_len] = '\0';

  return httpResponse;
}

void httpResponse_destroy(httpResponse_t *httpResponse) {
  free(httpResponse->status);
  free(httpResponse->headers);
  free(httpResponse->html);
  free(httpResponse);
}
