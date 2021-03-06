CC = gcc
CFLAGS = -Wall -I.

OBJECTS = list.o object.o main.o storage.o
EXAMPLES = example example.o

PROGNAME = main

all: $(PROGNAME) $(EXAMPLES)
	valgrind ./$(PROGNAME)

$(PROGNAME): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

example: example.o
	ld -o example example.o

example.o: example.asm
	nasm -f elf example.asm -o example.o

clean:
	rm -f $(PROGNAME) $(OBJECTS) $(EXAMPLES)
