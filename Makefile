

CC=gcc

TCC=../../../tcc/tcc

C_ARGS = -g -Wall -Wextra -pedantic -O0
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



.PRECIOUS: $(OBJS_WIN) $(OBJS_LINUX)
.PHONY: loc tcc
.SILENT: loc



bin/win/%.o: src/%.c $(HEADERS)
	mkdir -p $(@D)
	$(CC) -c $< \
	      -o $(patsubst src/%.c,bin/win/%.o,$<) \
				-I $(INCLUDES) $(C_ARGS)
bin/linux/%.o: src/%.c $(HEADERS)
	mkdir -p $(@D)
	$(CC) -c $< \
	      -o $(patsubst src/%.c,bin/linux/%.o,$<) \
				-I $(INCLUDES) $(C_ARGS)

bin/%.exe: test/%.c $(OBJS_WIN)
	mkdir -p $(@D)
	$(CC) test/$*.c $(OBJS_WIN) -I $(INCLUDES) $(C_ARGS) -o bin/$*.exe

bin/%: test/%.c $(OBJS_LINUX)
	mkdir -p $(@D)
	$(CC) test/$*.c $(OBJS_LINUX) -I $(INCLUDES) $(C_ARGS) -o bin/$*


tcc/%.exe:
	mkdir -p bin/tcc
	$(TCC) test/$*.c src/*/*.c -I src -w -o bin/tcc/$*.exe


loc:
	echo "src"
	printf "  vm:    %4s\n" $(shell cat src/vm/* | wc -l)
	printf "  util:  %4s\n" $(shell cat src/util/* | wc -l)
	echo   "  -----------"
	printf "  total: %4s\n" $(shell cat src/*/* | wc -l)
	echo
	echo   "test"
	printf "  .c:    %4s\n" $(shell cat test/*.c | wc -l)
	printf "  .txt:  %4s\n" $(shell cat test/*.txt | wc -l)
	echo   "  -----------"
	printf "  total: %4s\n" $(shell cat test/* | wc -l)
