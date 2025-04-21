CC = gcc
CFLAGS = -g -Wall
OBJS = main.o parser.o builtins.o signals.o executor.o

smallsh: $(OBJS)
	$(CC) $(CFLAGS) -o smallsh $(OBJS)

main.o: main.c parser.h builtins.h signals.h
parser.o: parser.c parser.h
builtins.o: builtins.c builtins.h
signals.o: signals.c signals.h
executor.o: executor.c executor.h parser.h signals.h

clean:
	rm -f *.o smallsh




