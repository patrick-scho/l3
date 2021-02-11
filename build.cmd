Rem C:/prg/tcc/tcc src/*.c -o main.exe -I src

cl test/parse1.c src/vm/*.c -I src -Zi -w

cl test/vm_fib.c src/vm/*.c -I src -Zi -w