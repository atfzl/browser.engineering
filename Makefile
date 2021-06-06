INCLUDE_PATHS ?= -I/usr/local/opt/openssl/include
LIBRARY_PATHS ?= -L/usr/local/opt/openssl/lib
LIBRARY_NAMES ?= -lssl -lcrypto -lSDL2 -lSDL2_ttf -lSDL2_image
C_FLAGS ?= -Wall -Wextra -std=gnu18

OBJECT_FILES := $(shell find src -name "**.c" | sed "s/\.c//" | sed 's/$$/.o/' | sed "s/^src/build/")

#$(info    OBJECT_FILES is $(OBJECT_FILES))

release: build/a.out

debug: C_FLAGS += -g
debug: build/a.out

test: tests/*.c

tests/*.c: $(OBJECT_FILES)
	mkdir -p build
	cc $(C_FLAGS) -g -o build/test.out $@ $(OBJECT_FILES) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LIBRARY_NAMES) -lcmocka && ./build/test.out

build/a.out: main.c $(OBJECT_FILES)
	cc $(C_FLAGS) -o $@ main.c $(OBJECT_FILES) $(LIBRARY_PATHS) $(LIBRARY_NAMES)

build/%.o: src/%.c
	mkdir -p $(dir $@)
	cc $(C_FLAGS) -c -o $@ $< $(INCLUDE_PATHS)

run:
	./build/a.out

setup-git-hooks:
	ln -s ../../git-hooks/pre-commit .git/hooks
	chmod +x .git/hooks/pre-commit

remove-git-hooks:
	rm .git/hooks/pre-commit

.PHONY: clean, tests/*.c
clean:
	rm -rf ./build
