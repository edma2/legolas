CC = gcc
CFLAGS = -Wall -I.

OBJECTS = list.o object.o main.o

PROGNAME = legolas

all: $(PROGNAME)

$(PROGNAME): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

example: example.o
	ld -o example example.o

example.o: example.asm
	nasm -f elf example.asm -o example.o

clean:
	rm -f $(PROGNAME) $(OBJECTS) example example.o
