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
    s1[0] = 'h';
    s1[1] = 'e';
    s1[2] = 'l';
    s1[3] = 'l';
    s1[4] = 'o';
    s1[5] = '\0';

    char* s2 = malloc(6);
    s2[0] = 'w';
    s2[1] = 'o';
    s2[2] = 'r';
    s2[3] = 'l';
    s2[4] = 'd';
    s2[5] = '\0';

    strcat_resize(&s1, s2);

    printf("%s\n", s1);

    return 0;
}