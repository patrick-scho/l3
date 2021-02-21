

CC=gcc

C_ARGS = -Wall -pedantic

INCLUDES=
INCLUDES += src

OBJS=
OBJS += $(patsubst src/%.c, bin/%.o, $(wildcard src/vm/*.c))
OBJS += $(patsubst src/%.c, bin/%.o, $(wildcard src/parse/*.c))
OBJS += $(patsubst src/%.c, bin/%.o, $(wildcard src/file/*.c))

bin/%.o: src/%.c
	$(CC) -c $< \
	      -o $(patsubst src/%.c,bin/%.o,$<) \
				-I $(INCLUDES) $(C_ARGS)

bin/%.exe: test/%.c $(OBJS)
	$(CC) test/$*.c $(OBJS) -I $(INCLUDES) $(C_ARGS)
