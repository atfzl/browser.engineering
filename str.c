#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char* s1 = malloc(6);
    strcpy(s1, "hello");

    char* s2 = malloc(6);
    strcpy(s2, "world");

    s1 = realloc(s1, strlen(s1) + strlen(s2) + 1);
    strcat(s1, s2);

    printf("%s\n", s1);

    return 0;
}
