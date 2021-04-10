CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -g

all: life

life: life.o universe.o
	$(CC) -o life life.o universe.o -lncurses

life.o: life.c
	$(CC) $(CFLAGS) -c life.c

universe.o: universe.c
	$(CC) $(CFLAGS) -c universe.c

clean:
	rm -f life *.o

valgrind: all
	valgrind -s --leak-check=full ./life -i lists/Hertz-oscillator.txt -s -o out

format:	life.c universe.c
	clang-format -i -style=file life.c
	clang-format -i -style=file universe.c
	clang-format -i -style=file universe.h

	
