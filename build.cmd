Rem C:/prg/tcc/tcc C:/prg/cpp/libs/vec/src/vec.c -c -o bin/vec.o
Rem C:/prg/tcc/tcc src/main.c bin/vec.o -o main.exe -I C:/prg/cpp/libs/vec/src

Rem cl C:/prg/cpp/libs/vec/src/vec.c -c -Fo:bin/vec.o
cl src/*.c bin/vec.obj -I C:/prg/cpp/libs/vec/src -Zi