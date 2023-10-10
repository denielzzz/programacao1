main: main.o liblista_ordenada.o
	gcc main.o liblista_ordenada.o -o main

main.o : main.c
	gcc -g -Wall -Wextra -c main.c

liblista_ordenada.o : liblista_ordenada.c
	gcc -g -Wall -Wextra -c liblista_ordenada.c

clean :
	rm *.o main