#include "code.h"
#include "endian.h"
#include "io.h"
#include "tools.h"
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
            //Print decompression statistics to stderr
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
        puts("   Decompresses files using the LZ78 decompression algorithm.");
        puts("   Used with files compressed with the corresponding encoder.");
        puts("USAGE");
        puts("   ./dencode [-vh] [-i input] [-o output]");
        puts("");
        puts("OPTIONS");
        puts("   -v         Display decompression statistics");
        puts("   -i input   Specify input to decompress (default: stdin)");
        puts("   -o output  Specify output of decompressed input (default: stdout)");
        puts("   -h         Display program help and usage");

        close(infile);
        close(outfile);
        exit(1);
    }

    //------------------- header reading -------------------

    FileHeader header = { 0, 0 };

    //reads the header from the compressed file
    read_header(infile, &header);

    //fixes the enianess of the header parameters if the computer is big endian
    fix_endianess(&header);

    if (header.magic != MAGIC) {
        fprintf(stderr, "Bad magic number.\n");
        close(infile);
        close(outfile);
        exit(1);
    }

    //set permission status of outfile to infile's permission status
    fchmod(outfile, header.protection);

    //------------------- decompression -------------------

    WordTable *table = wt_create();
    if (table == NULL) {
        close(infile);
        close(outfile);
        fprintf(stderr, MALLOC_ERR);
        exit(1);
    }

    uint8_t curr_sym = 0;

    uint16_t curr_code = 0;
    uint16_t next_code = START_CODE;

    while (read_pair(infile, &curr_code, &curr_sym, bitlen(next_code)) == true) {

        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        if (table[next_code] == NULL) {
            wt_delete(table);
            close(infile);
            close(outfile);
            fprintf(stderr, MALLOC_ERR);
            exit(1);
        }

        write_word(outfile, table[next_code]);
        next_code += 1;
        if (next_code == MAX_CODE) {
            wt_reset(table);
            next_code = START_CODE;
        }
    }

    flush_words(outfile);

    wt_delete(table);

    //------------------- print statistics -------------------
    if (stats == true) {
        struct stat statbuf;
        fstat(infile, &statbuf);

        uint64_t comp_fs = bytes(total_bits);
        uint64_t uncomp_fs = total_syms;
        double space_saving = 100 * (1 - ((double) comp_fs / uncomp_fs));

        fprintf(stderr, "Compressed file size: %" PRIu64 " bytes\n", comp_fs);
        fprintf(stderr, "Uncompressed file size: %" PRIu64 " bytes\n", uncomp_fs);
        fprintf(stderr, "Space saving: %4.2lf%%\n", space_saving);
    }
    //------------------- close files -------------------
    close(infile);
    close(outfile);
}
