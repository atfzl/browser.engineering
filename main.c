#include <stdio.h>
#include <stdlib.h>

#include "src/graphics.h"
#include "src/http.h"

int ENABLE_DEBUG = 0;
void init_debug() { ENABLE_DEBUG = (getenv("DEBUG") != NULL); }
#define DEBUG_INFO(format, ...) \
    if (ENABLE_DEBUG) fprintf(stdout, "DEBUG: " format, __VA_ARGS__)

int main() {
    init_debug();

    char url[] = "https://example.org/index.html";
    struct request_response_type response;

    request(url, &response);

    DEBUG_INFO("%s\n", url);

    const char* htmlText = lex(response.html);

    DEBUG_INFO("%s\n", htmlText);

    graphics_main(htmlText);

    free(response.status);
    free(response.headers);
    free(response.html);

    return EXIT_SUCCESS;
}
