#include <stdio.h>
#include <stdlib.h>

int length(char *s) {
    int l = 0;
    while (*s != '\0') {
        ++s;
        ++l;
    }
    return l;
}

int main() {
    char url[] = "http://example.org/index.html";

    printf("%lu\n", sizeof(url));

    return EXIT_SUCCESS;
}
