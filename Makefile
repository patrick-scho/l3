

CC=gcc

C_ARGS = -g -Wall -Wextra -pedantic
C_ARGS += -Werror

C_ARGS += -Wno-pointer-to-int-cast
C_ARGS += -Wno-int-to-pointer-cast
C_ARGS += -Wno-int-conversion
C_ARGS += -Wno-unused-function
C_ARGS += -Wno-unused-parameter
C_ARGS += -Wno-missing-field-initializers

INCLUDES=
INCLUDES += src

HEADERS=
HEADERS += $(wildcard src/*/*.h)

OBJS_WIN=
OBJS_WIN += $(patsubst src/%.c, bin/win/%.o, $(wildcard src/*/*.c))

OBJS_LINUX=
OBJS_LINUX += $(patsubst src/%.c, bin/linux/%.o, $(wildcard src/*/*.c))

bin/win/%.o: src/%.c $(HEADERS)
	mkdir -p $(patsubst src/%, bin/win/%, $(wildcard src/*/))
	$(CC) -c $< \
	      -o $(patsubst src/%.c,bin/win/%.o,$<) \
				-I $(INCLUDES) $(C_ARGS)
bin/linux/%.o: src/%.c $(HEADERS)
	mkdir -p $(patsubst src/%, bin/linux/%, $(wildcard src/*/))
	$(CC) -c $< \
	      -o $(patsubst src/%.c,bin/linux/%.o,$<) \
				-I $(INCLUDES) $(C_ARGS)

bin/%.exe: test/%.c $(OBJS_WIN)
	$(CC) test/$*.c $(OBJS_WIN) -I $(INCLUDES) $(C_ARGS) -o bin/$*.exe

bin/%: test/%.c $(OBJS_LINUX)
	$(CC) test/$*.c $(OBJS_LINUX) -I $(INCLUDES) $(C_ARGS) -o bin/$*




.PHONY: loc
.SILENT: loc

loc:
	echo "vm:    $(shell cat src/vm/* | wc -l)"
	echo "parse: $(shell cat src/parse/* | wc -l)"
	echo "file:  $(shell cat src/file/* | wc -l)"
	echo "total: $(shell cat src/vm/* src/parse/* src/file/* | wc -l)"