#include "io.h"

#include "code.h"
#include "endian.h"
#include "tools.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static uint8_t bitbuf[BLOCK];
static uint16_t bit_index = 0;

static uint8_t symbuf[BLOCK];
static uint16_t sym_index = 0;

uint64_t total_bits = 0;
uint64_t total_syms = 0;

// DEBUGGING FUNCTION
// prints an array as characters
// @param arr - the array
// @param length - the length of the array
void print_arr_c(uint8_t *arr, uint16_t length) {
    printf("[");
    for (uint8_t i = 0; i < length; i++) {
        // 10 means new line
        if (arr[i] != 10) {
            printf("%c, ", arr[i]);
        } else {
            printf("\\n, ");
        }
    }
    printf("]\n");

    return;
}

// DEBUGGING FUNCTION
// prints an array as integers
// @param arr - the array
// @param length - the length of the array
void print_arr_d(uint8_t *arr, uint16_t length) {
    printf("[");
    for (uint8_t i = 0; i < length; i++) {
        printf("%d, ", arr[i]);
    }
    printf("]\n");

    return;
}

// reads a specified amount of bytes from a text file into a buffer
// @param buf - the buffer that stores the read bytes
// @param to_read - the amount of bytes needed to be read from the infile
// @return - number of bytes read in total
int read_bytes(int infile, uint8_t *buf, int to_read) {

    int total = 0; //total bytes read

    int bytes = 0; //the # of bytes just read

    //because read() does not guarentee to read all of the bytes specified, we
    //need to call it in a loop
    while (to_read > 0 && (bytes = read(infile, buf, to_read)) > 0) {

        //increment total bytes read by the bytes just read
        total += bytes;

        //decrement the # of bytes that need to be read in by the # of bytes we just read
        to_read -= bytes;

        //moves the buffer pointer so that we don't rewrite the bytes we just
        //read when we call the loop again
        buf += bytes;
    }

    return total;
}

// writes a specified amount of bytes into a text file from a buffer
// @param buf - the buffer that stores the bytes needed to be written
// @param to_write - the amount of bytes needed to be written into the outfile
// @return - number of bytes written in total
int write_bytes(int outfile, uint8_t *buf, int to_write) {

    int total = 0; //total bytes written

    int bytes = 0; //the # of bytes just written

    //because write() does not guarentee to write all of the bytes specified, we
    //need to call it in a loop
    while (to_write > 0 && (bytes = write(outfile, buf, to_write)) > 0) {

        //increment total bytes read by the bytes just written
        total += bytes;

        //decrement the # of bytes that need to be written by the # of bytes we just wrote
        to_write -= bytes;

        //moves the buffer pointer so that we don't rewrite the bytes we just
        //wrote when we call the loop again
        buf += bytes;
    }

    return total;
}

// reads a header from an infile and stores it inside a pointer
// @param infile - the file that the header will be read from
// @param header - the pointer where the read in header will be stored
void read_header(int infile, FileHeader *header) {
    //reads the magic and protection data into a FileHeader ADT
    read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));

    //increase total bits processed by the amount of bits in a file header
    total_bits += (sizeof(FileHeader)) * 8;

    return;
}

// writes a header to the outfile
// @param outfile - the file that the header will be written to
// @param header - the header that will be written to the outfile
void write_header(int outfile, FileHeader *header) {
    //writes the magic and protection data from a FileHeader ADT, into the outfile
    write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));

    //increase total bits processed by the amount of bits in a file header
    total_bits += (sizeof(FileHeader)) * 8;

    return;
}

// reads the next symbol from the infile and sets it to the passed in sym pointer
// @param inflile - the file being read from
// @param sym - the pointer that the next symbol will be written to
// @return - true if there are more bytes to read in after this one
//			 false if there are no more bytes to read in after this one
bool read_sym(int infile, uint8_t *sym) {

    static uint16_t end = 0; // the amount of bytes avalible in the symbuf + 1

    //if symbuf is empty, fill it
    if (sym_index == 0) {
        int bytes_read = read_bytes(infile, symbuf, BLOCK);

        end = bytes_read + 1;
    }

    //set sym to the next sym in the symbuf
    *sym = symbuf[sym_index];

    //increment the total symbols proccessed
    total_syms++;

    sym_index++;

    //if symbuf is full, empty it
    if (sym_index == BLOCK) {
        sym_index = 0;
    }

    if (sym_index == end) {
        //there are no more bytes left to read in the file
        return false;
    }

    return true;
}

// writes a code and a symbol to an outfile in a pair
// @param outfile - the file we are writing the pair to
// @param code - the code
// @param bitlen - the length in bits of the code
// @param sym - the symbol
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {

    //if computer is big endian, swap the code's bits
    if (big_endian()) {
        code = swap16(code);
    }

    //copies each bit in 'code' to 'bitbuf'
    for (int i = 0; i < bitlen; i++) {
        //copy the bit
        if (bt_16_get_bit(code, i) == 1) {
            bt_buf_set_bit(bitbuf, bit_index);
        } else { // bit == 0
            bt_buf_clr_bit(bitbuf, bit_index);
        }

        bit_index += 1;
        //if the buffer is filled up, empty it into the outfile to allow
        //more bits to be read in
        if (bit_index == BLOCK * 8) {
            write_bytes(outfile, bitbuf, bytes(bit_index));
            bit_index = 0;
        }
    }

    //copies each bit in 'sym' to 'bitbuf' so that code and sym are next to eachother
    for (int i = 0; i < 8; i++) {
        //copy the bit
        if (bt_8_get_bit(sym, i) == 1) {
            bt_buf_set_bit(bitbuf, bit_index);
        } else { // bit == 0
            bt_buf_clr_bit(bitbuf, bit_index);
        }

        bit_index += 1;
        //if the buffer is filled up, empty it into the outfile to allow
        //more bits to be read in
        if (bit_index == BLOCK * 8) {
            write_bytes(outfile, bitbuf, bytes(bit_index));
            bit_index = 0;
        }
    }

    //increase total bits by the amount of bits written
    total_bits += bitlen + 8;

    return;
}

// empties bitbuf into the outfile
// @param outfile - the outfile being written to
void flush_pairs(int outfile) {
    if (bit_index == 0) {
        return;
    }

    write_bytes(outfile, bitbuf, bytes(bit_index));
    bit_index = 0;

    return;
}

// reads a code and a symbol from an infile
// @param infile - the file that the symbol and code are being read from
// @param code - the pointer to where the read in code should be stored
// @param sym - the pointer to where the read in sym should be stored
// @param bitlen - the length in bits of the code being read in
// @return - true if there are more codes to be read
//			 false if the final code was just read
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    assert(code && sym);

    //create a code that will eventually overwrite the passed in 'code'
    uint16_t read_in_code = 0;

    //copies 'bitlen' amount of bits from infile to 'bitbuf'
    for (int i = 0; i < bitlen; i++) {
        //if bitbuf is empty, fill it up (read_bytes does not change bit_index)
        if (bit_index == 0) {
            int bytes_read = read_bytes(infile, bitbuf, BLOCK);

            //if there are no more bytes to read and we haven't read 'bitlen'
            //amount of bits, abort the program bc the file was corrupted
            if (bytes_read == 0) {
                fprintf(stderr, "Corrupted File: pairs do not match exactly");
                return false;
            }
        }

        //copy the bit at bit_index of 'bitbuf' to 'read_code'
        if (bt_buf_get_bit(bitbuf, bit_index) == 1) {
            bt_16_set_bit(&read_in_code, i);
        } else { // bit == 0
            bt_16_clr_bit(&read_in_code, i);
        }

        bit_index += 1;

        //if the end of 'bitbuf' is reached, prepare to overwrite 'bitbuf'
        //with the next bytes to be read
        if (bit_index == BLOCK * 8) {
            bit_index = 0;
        }
    }

    //if computer is big endian, swap the code's bits
    if (big_endian()) {
        read_in_code = swap16(read_in_code);
    }

    *code = read_in_code;

    //create a symbol that will eventually overwrite the passed in 'sym'
    uint8_t read_in_sym = 0;

    for (int i = 0; i < 8; i++) {
        //if bitbuf is empty, fill it up (read_bytes does not change bit_index)
        if (bit_index == 0) {
            int bytes_read = read_bytes(infile, bitbuf, BLOCK);

            //if there are no more bytes to read and we haven't read 'bitlen'
            //amount of bits, abort the program bc the file was corrupted
            if (bytes_read == 0) {
                fprintf(stderr, "Corrupted File: pairs do not match exactly");
                return false;
            }
        }

        //copy the bit at bit_index of 'bitbuf' to 'sym'
        if (bt_buf_get_bit(bitbuf, bit_index) == 1) {
            bt_8_set_bit(&read_in_sym, i);
        } else { // bit == 0
            bt_8_clr_bit(&read_in_sym, i);
        }

        bit_index += 1;

        //if the end of 'bitbuf' is reached, prepare to overwrite 'bitbuf'
        //with the next bytes to be read
        if (bit_index == BLOCK * 8) {
            bit_index = 0;
        }
    }

    *sym = read_in_sym;

    //increase total bits by the amount of bits read
    total_bits += bitlen + 8;

    if (read_in_code == STOP_CODE) {
        //there are no more pairs left to read after this one
        return false;
    }

    //there are more pairs left to read after this one
    return true;
}

// writes a word to a file
// @param outfile - the file the word will be written to
// @param w - the word being written
void write_word(int outfile, Word *w) {
    assert(w);

    //iterate through the symbols in 'w'
    for (uint32_t i = 0; i < w->len; i++) {
        uint8_t sym = w->syms[i];

        //fill up the buf with the 'sym's
        symbuf[sym_index] = sym;

        sym_index += 1;
        total_syms++; //increment total syms proccessed

        //empty the buf if it gets filled up
        if (sym_index == BLOCK) {
            write_bytes(outfile, symbuf, sym_index);
            sym_index = 0;
        }
    }

    return;
}

// empties all the words currently in the symbuf to a file
// @param outfile - the file
void flush_words(int outfile) {
    if (sym_index == 0) {
        return;
    }

    write_bytes(outfile, symbuf, sym_index);
    sym_index = 0;

    return;
}
