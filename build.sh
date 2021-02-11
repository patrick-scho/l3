#!/bin/sh

gcc test/parse1.c src/vm/*.c src/parse/*.c src/file/*.c -I src -w -o bin/parse1

gcc test/vm_fib.c src/vm/*.c src/parse/*.c src/file/*.c -I src -w -o bin/vm_fib