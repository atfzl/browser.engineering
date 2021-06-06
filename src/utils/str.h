#pragma once

#include <stdlib.h>

int str_startsWith(char *str, const char *prefix);

void str_trimStart(char **str, const char *prefix);

int str_indexOf(const char *str, char c);

char *str_slice(const char *str, size_t startIndex, size_t endIndex);
