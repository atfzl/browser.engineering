INC_DIRS ?= -I/usr/local/opt/openssl/include
LIB_DIRS ?= -L/usr/local/opt/openssl/lib
LIBS_ARG ?= -lssl -lcrypto -lSDL2 -lSDL2_ttf -lSDL2_image
CC_ARG ?= -Wall -std=c11

build/a.out: main.c build/http.o
	CC $(CC_ARG) -o build/a.out main.c build/http.o $(LIB_DIRS) $(LIBS_ARG)
build/http.o: src/http.c
	mkdir -p ./build
	CC $(CC_ARG) -c -o build/http.o src/http.c $(INC_DIRS)

.PHONY: clean
clean:
	rm -rf ./build
