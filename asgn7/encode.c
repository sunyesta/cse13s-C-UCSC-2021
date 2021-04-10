#include "code.h"
#include "endian.h"
#include "io.h"
#include "tools.h"
#include "trie.h"
#include "word.h"

#include <assert.h>
#include <fcntl.h> // for open()
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for close()

//file status libraries
#include <sys/stat.h>
#include <sys/types.h>

#define OPTIONS    "vi:o:h"
#define MALLOC_ERR "malloc error\n"

// fixes the values inside a file header depending if a computer is big or little endian
// @param h - the file header
void fix_endianess(FileHeader *h) {
    //check if system is big endian
    if (big_endian() == true) {
        //if it is big endian, swap the endianess
        h->magic = swap32(h->magic);
        h->protection = swap16(h->protection);
    }
    return;
}

int main(int argc, char **argv) {
    int infile = 0; //stdin
    int outfile = 1; //stdout
    bool stats = false;
    bool help = false;
    //parses the commandline options
    int opt;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v':
            //Print compression statistics to stderr
            stats = true;
            break;
        case 'i':
            //open an infile so that it can be read from
            infile = open(optarg, O_RDONLY, 0600);
            break;
        case 'o':
            //open an outfile so that it:
            //		-can be written in
            //		-can be created if it doesn't exist
            //		-will be emptied if it has contents inside of it
            outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0600);
            break;
        case 'h': help = true; break;
        default: help = true; break;
        }
    }

    //------------------- file error checking -------------------

    if (infile == -1) {
        close(outfile);
        fprintf(stderr, "failed to open infile\n");
        exit(1);
    }

    if (outfile == -1) {
        close(infile);
        fprintf(stderr, "failed to open outfile\n");
        exit(1);
    }

    //------------------- help printing -------------------

    if (help == true) {
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

        close(infile);
        close(outfile);
        exit(1);
    }

    //------------------- header writing -------------------

    FileHeader *header = (FileHeader *) calloc(1, sizeof(FileHeader));

    header->magic = MAGIC;

    struct stat statbuf;
    //get permission status of infile
    fstat(infile, &statbuf);
    //set permission status of outfile to infile's permission status
    fchmod(outfile, statbuf.st_mode);
    header->protection = statbuf.st_mode;

    fix_endianess(header);

    //writes the header to the compressed file
    write_header(outfile, header);

    free(header);

    //------------------- compression -------------------

    TrieNode *root = trie_create();
    if (root == NULL) {
        close(infile);
        close(outfile);
        fprintf(stderr, MALLOC_ERR);
        exit(1);
    }

    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;

    uint8_t curr_sym = 0;
    uint8_t prev_sym = 0;
    uint16_t next_code = START_CODE;

    while (read_sym(infile, &curr_sym) == true) {
        TrieNode *next_node = trie_step(curr_node, curr_sym);

        if (next_node != NULL) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            write_pair(outfile, curr_node->code, curr_sym, bitlen(next_code));

            curr_node->children[curr_sym] = trie_node_create(next_code);
            if (curr_node->children[curr_sym] == NULL) {
                trie_delete(root);
                close(infile);
                close(outfile);
                fprintf(stderr, MALLOC_ERR);
                exit(1);
            }

            curr_node = root;
            next_code = next_code + 1;
        }
        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }

        prev_sym = curr_sym;
    }

    if (curr_node != root) {
        write_pair(outfile, prev_node->code, prev_sym, bitlen(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }
    write_pair(outfile, STOP_CODE, 0, bitlen(next_code));
    flush_pairs(outfile);

    trie_delete(root);

    //------------------- print statistics -------------------
    if (stats == true) {
        uint64_t comp_fs = bytes(total_bits);
        uint64_t uncomp_fs = total_syms - 1; // -1 for EOF
        double space_saving = 100 * (1 - ((double) comp_fs / uncomp_fs));

        fprintf(stderr, "Compressed file size: %" PRIu64 " bytes\n", comp_fs);
        fprintf(stderr, "Uncompressed file size: %" PRIu64 " bytes\n", uncomp_fs);
        fprintf(stderr, "Space saving: %4.2lf%%\n", space_saving);
    }

    //------------------- close files -------------------
    close(infile);
    close(outfile);

    return 0;
}
