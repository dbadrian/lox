CC = gcc
CFLAGS  = -g -Wall


default: clox

clox:  clox.o compiler.o chunk.o debug.o memory.o value.o vm.o scanner.o linenoise.o
	$(CC) $(CFLAGS) -o clox clox.o compiler.o chunk.o debug.o memory.o value.o vm.o scanner.o linenoise.o

all: clox

clean:
	$(RM) clox *.o *~