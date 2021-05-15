#include <stdio.h>
#include <stdlib.h>

#include "src/http.h"

int main() {
    char url[] = "https://example.org/index.html";
    struct request_response_type response;

    request(url, &response);

    const char* x = lex(response.html);

    printf("%s\n", x);

    free(response.status);
    free(response.headers);
    free(response.html);

    return 0;
    return EXIT_SUCCESS;
}
