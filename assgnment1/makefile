all: test

test: main.o list.o
	gcc -g -Wall -Werror -o test main.o list.o

main.o: main.c list.h list.c
	gcc -g -Wall -Werror -c main.c 

list.o: list.c list.h
	gcc -g -Wall -Werror -c list.c 

clean: 
	rm -f test *.o