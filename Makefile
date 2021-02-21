

CC=gcc

C_ARGS = -g #-Wall -pedantic

INCLUDES=
INCLUDES += src

HEADERS=
HEADERS += $(wildcard src/*/*.h)

OBJS=
OBJS += $(patsubst src/%.c, bin/%.o, $(wildcard src/*/*.c))

bin/%.o: src/%.c $(HEADERS)
	$(CC) -c $< \
	      -o $(patsubst src/%.c,bin/%.o,$<) \
				-I $(INCLUDES) $(C_ARGS)

bin/%.exe: test/%.c $(OBJS)
	$(CC) test/$*.c $(OBJS) -I $(INCLUDES) $(C_ARGS) -o bin/$*.exe

bin/%: test/%.c $(OBJS)
	$(CC) test/$*.c $(OBJS) -I $(INCLUDES) $(C_ARGS) -o bin/$*

wc:
	cat src/vm/* | wc -l
	cat src/parse/* | wc -l
	cat src/file/* | wc -l
	cat src/vm/* src/parse/* src/file/* | wc -l
