#include <stdio.h>
#include <stdlib.h>

#include "src/graphics.h"
#include "src/http.h"

int main() {
    char url[] = "https://example.org/index.html";
    struct request_response_type response;

    request(url, &response);

    const char* htmlText = lex(response.html);

    printf("%s\n", htmlText);

    graphics_main(htmlText);

    free(response.status);
    free(response.headers);
    free(response.html);

    return EXIT_SUCCESS;
}
