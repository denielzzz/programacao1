#
# A simple makefile for managing build of project composed of C source files.
#


# It is likely that default C compiler is already gcc, but explicitly
# set, just to be sure
CC = gcc

# The CFLAGS variable sets compile flags for gcc:
#  -g        compile with debug information
#  -Wall     give verbose compiler warnings

CFLAGS = -g  -Wall -Wextra -pedantic
LDFLAGS = -lm

main: listaLib.o main.o
	$(CC) main.o listaLib.o -o main $(LDFLAGS)

main.o : main.c	
	$(CC) $(CFLAGS) -c main.c

cartesiano.o : listaLib.c
	$(CC) $(CFLAGS) -c listaLib.c

clean :
	rm *.o main

