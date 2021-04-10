CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -g

all: sorting


sorting: sorting.o bubble.o shell.o quick.o stack.o heap.o set.o tools.o
	$(CC) -o sorting sorting.o bubble.o shell.o quick.o stack.o heap.o set.o tools.o

sorting.o: sorting.c
	$(CC) $(CFLAGS) -c sorting.c

bubble.o: bubble.c
	$(CC) $(CFLAGS) -c bubble.c

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c

quick.o: quick.c
	$(CC) $(CFLAGS) -c quick.c

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c

set.o: set.c
	$(CC) $(CFLAGS) -c set.c

tools.o: tools.c
	$(CC) $(CFLAGS) -c tools.c

clean:
	rm -f sorting *.o

valgrind: all
	valgrind -s --leak-check=full ./sorting -i lists/Hertz-oscillator.txt -s -o out

format:	all
	clang-format -i -style=file *.[ch]

