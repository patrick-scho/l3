

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


define msg
# L3
## LOCs
loc vm:    $(shell cat src/vm/* | wc -l)
loc parse: $(shell cat src/parse/* | wc -l)
loc file:  $(shell cat src/file/* | wc -l)
loc total: $(shell cat src/vm/* src/parse/* src/file/* | wc -l)
endef

.PHONY: readme
.SILENT: readme
	
readme: export msg:=$(msg)
readme:
	echo "$${msg}" > README.md