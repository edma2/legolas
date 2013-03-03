main: main.c elfobject.c
	gcc -Wall -I . main.c elfobject.c -o main

example.o: example/test.asm
	nasm -f elf example/test.asm -o example/test.o

example: example.o
	ld -o example/test example/test.o

clean:
	rm -f main example/test example/test.o
