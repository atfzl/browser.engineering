struct request_response_type {
    char* status;
    char* headers;
    char* html;
};

void request(char* url, struct request_response_type* response);

const char* lex(const char* html);
