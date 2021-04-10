CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -g

all: gen dec err



gen: generator.o hamming.o bm.o
	$(CC) -o gen generator.o hamming.o bm.o

dec: decoder.o hamming.o bm.o
	$(CC) -o dec decoder.o hamming.o bm.o

err: error.o
	$(CC) -o err error.o



generator.o: generator.c
	$(CC) $(CFLAGS) -c generator.c

decoder.o: decoder.c
	$(CC) $(CFLAGS) -c decoder.c

bm.o: bm.c
	$(CC) $(CFLAGS) -c bm.c

hamming.o: hamming.c
	$(CC) $(CFLAGS) -c hamming.c

error.o: error.c
	$(CC) $(CFLAGS) -c error.c


clean:
	rm -f gen dec err *.o

valgrind: all
	valgrind -s --leak-check=full ./gen -i lists/Hertz-oscillator.txt -s -o out
	valgrind -s --leak-check=full ./dec -i lists/Hertz-oscillator.txt -s -o out

format:
	clang-format -i -style=file *.[ch]
