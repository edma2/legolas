legolas: legolas.c
	gcc -Wall -I . legolas.c -o legolas

test: legolas
	./legolas example/test.o test-output

example.o: example/test.asm
	nasm -f elf example/test.asm -o example/test.o

example: example.o
	ld -o example/test example/test.o

clean:
	rm -f legolas test-output example/test example/test.o
