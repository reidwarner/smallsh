CC = gcc
CFLAGS = -g -Wall
OBJS = main.o parser.o builtins.o signals.o

smallsh: $(OBJS)
	$(CC) $(CFLAGS) -o smallsh $(OBJS)

main.o: main.c parser.h builtins.h signals.h
parser.o: parser.c parser.h
builtins.o: builtins.c builtins.h
signals.o: signals.c signals.h

clean:
	rm -f *.o smallsh
