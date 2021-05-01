#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strcat_resize(char** s1, const char* s2) {
    char* result = malloc(strlen(*s1) + strlen(s2) + 1);
    strcpy(result, *s1);
    strcat(result, s2);
    free(*s1);
    *s1 = result;
}

int main(int argc, char* argv[]) {
    char* s1 = malloc(6);
    strcpy(s1, "hello");

    char* s2 = malloc(6);
    strcpy(s2, "world");

    strcat_resize(&s1, s2);

    printf("%s\n", s1);

    return 0;
}
