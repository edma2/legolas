legolas: legolas.c
	gcc -Wall -I . legolas.c -o legolas

test: legolas
	./legolas example/test.o test-output

clean:
	rm -f legolas test-output
