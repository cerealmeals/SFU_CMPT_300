CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lpthread
PROG = s-talk
HDRS = list.h sleep.h receiver.h sender.h
SRCS = main.c list.c sleep.c receiver.c sender.c
## This incantation says that the object files
## have the same name as the .c files, but with .o
OBJS = $(SRCS:.c=.o)
## This is the first rule (the default)
## Build the program from the three .o's
$(PROG) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG) $(LDFLAGS)
## Rules for the source files -- these do not have
## second build rule lines, so they will use the
## default build rule to compile X.c to make X.o
main.o : main.c list.h sleep.h
list.o : list.h 
sleep.o : sleep.h
# test.o : test.h
## Remove all the compilation and debugging files
clean :
	rm -f core $(PROG) $(OBJS)
## Build tags for these sources
TAGS : $(SRCS) $(HDRS)
	etags -t $(SRCS) $(HDRS)