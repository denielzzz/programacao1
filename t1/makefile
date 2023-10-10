CFLAGS = -g -Wall -Wextra -Werror -pedantic -std=c99

all: agenda.o libagenda.o libfirma.o
	gcc -o agenda agenda.o libagenda.o libfirma.o 

agenda.o: agenda.c
	gcc -c $(CFLAGS) agenda.c

libagenda.o: libagenda.c
	gcc -c $(CFLAGS) libagenda.c

libfirma: libfirma.c
	gcc -c $(CFLAGS) libfirma.c

clean:
	rm -f *.o agenda