#include "./htmlShow.h"
#include <stdbool.h>
#include <string.h>

string_t *htmlShow(char *rawHTML) {
  string_t *result = string_init();

  bool inAngle = false;
  size_t rawHTMLLength = strlen(rawHTML);
  for (size_t i = 0; i < rawHTMLLength; ++i) {
    char c = rawHTML[i];
    if (c == '<') {
      inAngle = true;
    } else if (c == '>') {
      inAngle = false;
    } else if (!inAngle) {
      string_push(result, c);
    }
  }

  return result;
}
