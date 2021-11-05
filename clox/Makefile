CC = gcc
CFLAGS  = -g -Wall


default: clox

clox:  main.o compiler.o chunk.o debug.o memory.o value.o vm.o scanner.o linenoise.o
	$(CC) $(CFLAGS) -o clox main.o compiler.o chunk.o debug.o memory.o value.o vm.o scanner.o linenoise.o


clean:
	$(RM) clox *.o *~