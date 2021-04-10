CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -g

all: banhammer


banhammer: banhammer.o ll.o node.o hash.o speck.o bv.o bf.o parser.o
	$(CC) -o banhammer banhammer.o ll.o node.o hash.o speck.o bv.o bf.o parser.o


test.o: test.c
	$(CC) $(CFLAGS) -c test.c

banhammer.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c

speck.o: speck.c #
	$(CC) $(CFLAGS) -c speck.c

hash.o: hash.c #
	$(CC) $(CFLAGS) -c hash.c

ll.o: ll.c #
	$(CC) $(CFLAGS) -c ll.c

node.o: node.c #
	$(CC) $(CFLAGS) -c node.c

bf.o: bf.c #
	$(CC) $(CFLAGS) -c bf.c

bv.o: bv.c #
	$(CC) $(CFLAGS) -c bv.c

parser.o: parser.c #
	$(CC) $(CFLAGS) -c parser.c

clean:
	rm -f banhammer err *.o

valgrind: all
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./banhammer 

format:
	clang-format -i -style=file *.[ch]
