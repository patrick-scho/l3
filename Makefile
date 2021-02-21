

CC=gcc

C_ARGS = -g #-Wall -pedantic

INCLUDES=
INCLUDES += src

HEADERS=
HEADERS += $(wildcard src/vm/*.h)
HEADERS += $(wildcard src/parse/*.h)
HEADERS += $(wildcard src/file/*.h)

OBJS=
OBJS += $(patsubst src/%.c, bin/%.o, $(wildcard src/vm/*.c))
OBJS += $(patsubst src/%.c, bin/%.o, $(wildcard src/parse/*.c))
OBJS += $(patsubst src/%.c, bin/%.o, $(wildcard src/file/*.c))

bin/%.o: src/%.c $(HEADERS)
	$(CC) -c $< \
	      -o $(patsubst src/%.c,bin/%.o,$<) \
				-I $(INCLUDES) $(C_ARGS)

bin/%.exe: test/%.c $(OBJS)
	$(CC) test/$*.c $(OBJS) -I $(INCLUDES) $(C_ARGS) -o bin/$*.exe

bin/%: test/%.c $(OBJS)
	$(CC) test/$*.c $(OBJS) -I $(INCLUDES) $(C_ARGS) -o bin/$*
