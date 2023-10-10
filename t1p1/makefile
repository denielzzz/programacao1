CFLAGS = -g -Wall -Wextra 

all: agenda.o libagenda.o
	gcc -o agenda agenda.o libagenda.o

agenda.o: agenda.c
	gcc -c $(CFLAGS) agenda.c

libagenda.o: libagenda.c
	gcc -c $(CFLAGS) libagenda.c

clean:
	rm -f *.o agenda