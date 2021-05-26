#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int str_startsWith(const char *str, const char *prefix) {
  size_t len = strlen(str);
  size_t prefixLen = strlen(prefix);

  // not possible to have prefix longer than string
  if (len < prefixLen) {
    return 0;
  }

  size_t j = 0;
  while (j < prefixLen && prefix[j] == str[j]) {
    ++j;
  }

  if (j == prefixLen) {
    return 1;
  }

  return 0;
}

int str_indexOf(const char *str, char c) {
  for (size_t i = 0; i < strlen(str); ++i) {
    if (str[i] == c) {
      return (int)i;
    }
  }

  return -1;
}

char *str_slice(const char *str, size_t startIndex, size_t endIndex) {
  if (startIndex < 0 || endIndex >= strlen(str) || startIndex > endIndex) {
    return NULL;
  }

  size_t newLength = endIndex - startIndex + 1;

  // +1 for \0
  char *newString = malloc((newLength + 1) * sizeof(char));

  size_t i = 0;
  for (size_t j = startIndex; j <= endIndex; ++j) {
    newString[i] = str[j];
    ++i;
  }

  newString[newLength] = '\0';

  return newString;
}

void str_trimStart(char **str, const char *prefix) {
  size_t len = strlen(*str);
  size_t prefixLen = strlen(prefix);

  if (len < prefixLen) {
    return;
  }

  size_t j = 0;
  while (j < prefixLen && prefix[j] == (char)**str) {
    ++(*str);
    ++j;
  }
}
