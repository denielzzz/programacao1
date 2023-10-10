CC = gcc
CFLAGS = -g  -Wall -Wextra -pedantic

main: libConfeitaria.o main.o
	$(CC) main.o libConfeitaria.o -o confeitaria

main.o : main.c	
	$(CC) $(CFLAGS) -c main.c

cartesiano.o : libConfeitaria.c
	$(CC) $(CFLAGS) -c libConfeitaria.c

clean :
	rm *.o confeitaria
