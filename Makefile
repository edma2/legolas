legolas: legolas.o
	gcc -I . legolas.o -o legolas

legolas.o:
	gcc -I . -c legolas.c

clean:
	rm legolas.o legolas
