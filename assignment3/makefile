## A simple makefile
CC = gcc
CFLAGS = -Wall -g -I/300/ass1
LDFLAGS = 
PROG = program
HDRS = list.h Structures_functions.h
SRCS = main.c list.c Structures_functions.c
## This incantation says that the object files
## have the same name as the .c files, but with .o
OBJS = $(SRCS:.c=.o)
## This is the first rule (the default)
## Build the program from the three .o's
$(PROG) : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(PROG)
## Rules for the source files -- these do not have
## second build rule lines, so they will use the
## default build rule to compile X.c to make X.o
main.o : main.c list.h Structures_functions.h
list.o : list.h
Structures_functions.o : Structures_functions.h
## Remove all the compilation and debugging files
clean :
	rm -f core $(PROG) $(OBJS)
## Build tags for these sources
TAGS : $(SRCS) $(HDRS)
	etags -t $(SRCS) $(HDRS)