CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -g

all: encode decode entropy

encode: encode.o io.o tools.o word.o trie.o
	$(CC) -o encode encode.o io.o tools.o word.o trie.o


decode: decode.o io.o tools.o word.o trie.o
	$(CC) -o decode decode.o io.o tools.o word.o trie.o

entropy: entropy.o
	$(CC) -o entropy entropy.o -lm

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c



code.o: code.c 
	$(CC) $(CFLAGS) -c code.c

endian.o: endian.c 
	$(CC) $(CFLAGS) -c endian.c

entropy.o: entropy.c 
	$(CC) $(CFLAGS) -c entropy.c

io.o: io.c
	$(CC) $(CFLAGS) -c io.c

trie.o: trie.c
	$(CC) $(CFLAGS) -c trie.c

word.o: word.c
	$(CC) $(CFLAGS) -c word.c

tools.o: tools.c
	$(CC) $(CFLAGS) -c tools.c




clean:
	rm -f encode decode entropy err *.o

format:
	clang-format -i -style=file *.[ch]

