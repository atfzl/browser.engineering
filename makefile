INC_DIRS ?= -I/usr/local/opt/openssl/include
LIB_DIRS ?= -L/usr/local/opt/openssl/lib
LIBS_ARG ?= -lssl -lcrypto -lSDL2 -lSDL2_ttf -lSDL2_image
CC_ARG ?= -Wall -std=c11

OBJECT_FILES := $(shell find src -name "**.c" | sed "s/\.c//" | sed 's/$$/.o/' | sed "s/^src/build/")

#$(info    OBJECT_FILES is $(OBJECT_FILES))

build/a.out: main.c $(OBJECT_FILES)
	CC $(CC_ARG) -o $@ main.c $(OBJECT_FILES) $(LIB_DIRS) $(LIBS_ARG)

build/%.o: src/%.c
	mkdir -p $(dir $@)
	CC $(CC_ARG) -c -o $@ $< $(INC_DIRS)

.PHONY: clean
clean:
	rm -rf ./build
