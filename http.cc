//
//  main.cpp
//  hello
//
//  Created by Atif Afzal on 4/9/21.
//  references:
//    https://browser.engineering/http.html
//    https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html
//    https://en.wikipedia.org/wiki/Berkeley_sockets
//

// namespace std
#include <iostream>
#include <string>

// import perror
#include <stdio.h>

// import exit
#include <stdlib.h>

// import assert
#include <cassert>

// import socket, AF_INET, SOCK_STREAM
#include <sys/socket.h>

// import inet_pton
#include <arpa/inet.h>

// import close
#include <unistd.h>

// import addrinfo, getaddrinfo
#include <netdb.h>

#define BUF_SIZE 2048

bool startsWith(std::string s, std::string prefix) {
    // https://stackoverflow.com/questions/1878001/how-do-i-check-if-a-c-stdstring-starts-with-a-certain-string-and-convert-a#answer-40441240
    return s.rfind(prefix, 0) == 0;
}

int main(int argc, const char * argv[]) {
    std::string url = "http://example.org/index.html";

    assert(startsWith(url, "http://"));

    // remove `http://` prefix
    url = url.substr(7);

    std::string host = "";
    std::string path = "";
    bool slashFound = false;
    for (unsigned int i = 0; i < url.length(); ++i) {
        char ch = url[i];
        if (ch == '/') {
            slashFound = true;
            continue;
        }
        if (!slashFound) {
            host.push_back(ch);
        } else {
            path.push_back(ch);
        }
    }
    std::cout << host << std::endl;
    std::cout << path << std::endl;

    // ----------
    
    struct addrinfo hints;
    struct addrinfo *result;

    // clear memory
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int s = getaddrinfo(host.c_str(), "http", &hints, &result);

    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    int SocketFD;

    SocketFD = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (SocketFD == -1) {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    if (connect(SocketFD, result->ai_addr, result->ai_addrlen) == -1) {
        perror("cannot connect to socket");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    std::string message = "GET /index.html HTTP/1.0\r\nHost: example.org\r\n\r\n";

    if (write(SocketFD, message.c_str(), message.length()) != message.length()) {
        perror("failed write");
        exit(EXIT_FAILURE);
    }
    
    char buf[BUF_SIZE];
    ssize_t nread = read(SocketFD, buf, BUF_SIZE);
    if (nread == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    printf("Received %zd bytes: %s\n", nread, buf);    

    close(SocketFD);

    return EXIT_SUCCESS;
}
