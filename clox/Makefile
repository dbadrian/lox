CC = gcc
CFLAGS  = -g -Wall


default: clox

clox:  main.o chunk.o debug.o memory.o
	$(CC) $(CFLAGS) -o clox main.o chunk.o debug.o memory.o


# countwords.o:  countwords.c scanner.h counter.h 
# 	$(CC) $(CFLAGS) -c countwords.c


# counter.o:  counter.c counter.h 
# 	$(CC) $(CFLAGS) -c counter.c

# # To create the object file scanner.o, we need the source files
# # scanner.c and scanner.h:
# #
# scanner.o:  scanner.c scanner.h 
# 	$(CC) $(CFLAGS) -c scanner.c

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean:
	$(RM) clox *.o *~