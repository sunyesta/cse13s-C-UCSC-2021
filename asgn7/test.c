#include "bit_tools.h"
#include "code.h"
#include "endian.h"
#include "io.h"
#include "trie.h"
#include "word.h"

#include <assert.h>
#include <fcntl.h> // for open()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for close()

//file status libraries
#include <sys/stat.h>
#include <sys/types.h>

#define OPTIONS "vi:o:h"

// fixes the values inside a file header depending if a computer is big or little endian
// @param h - the file header
void fix_endianess(FileHeader *h) {
    //check if system is little endian
    if (little_endian() == true) {
        return;
    }
    //if it is big endian, swap the endianess
    h->magic = swap32(h->magic);
    h->protection = swap16(h->protection);
    return;
}

// prints program usage instructions
void help() {
    puts("SYNOPSIS");
    puts("   Compresses files using the LZ78 compression algorithm.");
    puts("   Compressed files are decompressed with the corresponding decoder.");
    puts("USAGE");
    puts("   ./encode [-vh] [-i input] [-o output]");
    puts("");
    puts("OPTIONS");
    puts("   -v         Display compression statistics");
    puts("   -i input   Specify input to compress (default: stdin)");
    puts("   -o output  Specify output of compressed input (default: stdout)");
    puts("   -h         Display program help and usage");
}

int main(int argc, char **argv) {
    int infile = 0; //stdin
    int outfile = 1; //stdout
    //parses the commandline options
    int opt;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': break;
        case 'i': infile = open(optarg, O_RDONLY, 0600); break;
        case 'o': outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0600); break;
        case 'h': help(); exit(1);
        default: help(); exit(1);
        }
    }

    //error checking for files
    if (outfile == -1 || infile == -1) {
        fprintf(stderr, "failed to open infile or outfile\n");
        exit(1);
    }

    //------------------- testing -------------------

    TrieNode *root = trie_create();
    root->children[0] = trie_node_create(2);
    root->children[1] = trie_node_create(3);
    root->children[2] = trie_node_create(4);
    root->children[1]->children[2] = trie_node_create(52);
    root->children[52] = trie_node_create(52);
    print_tree(root);
    trie_node_delete(root->children[1]);
    //print_tree(root);
    trie_reset(root);
    //print_tree(root);

    return 0;
}
