#include "bm.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    uint8_t **mat;
};

//gets the amount of bytes needed for an amount of bits
//@param bits - the amount of bits
uint32_t bytes(uint32_t bits) {
    //if bits can perfectly fit into a number of bytes, then return bits/8
    if (bits % 8 == 0) {
        return bits / 8;

        //if bits doesn't perfectly fit into a number of bytes,
        //allocate 1 extra byte so that the remaining bits can have space
    } else {
        return (bits / 8) + 1;
    }
}

//creates a BitMatrix
//@param rows - the amount of rows in the matrix
//@param cols - the amount of cols in the matrix
//@return - a pointer to the ADT
BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *) calloc(1, sizeof(BitMatrix));
    if (m == NULL) {
        return NULL;
    }

    m->rows = rows;
    m->cols = cols;

    m->mat = (uint8_t **) calloc(rows, sizeof(uint8_t *));
    if (m->mat == NULL) {
        free(m);
        return NULL;
    }

    for (uint32_t r = 0; r < rows; r++) {
        m->mat[r] = (uint8_t *) calloc(bytes(cols), sizeof(uint8_t));
        if (m->mat[r] == NULL) {
            free(m);
            return NULL;
        }
    }

    return m;
}

//frees the BM's space
//@param m - the matrix
void bm_delete(BitMatrix **m) {
    for (uint32_t r = 0; r < (*m)->rows; r++) {
        free((*m)->mat[r]);
    }
    free((*m)->mat);
    free(*m);
    *m = NULL;
}

//returns the amount of rows in the matrix
//@param m - the matrix
uint32_t bm_rows(BitMatrix *m) {
    return m->rows;
}

//returns the amount of cols in the matrix
//@param m - the matrix
uint32_t bm_cols(BitMatrix *m) {
    return m->cols;
}

//sets a bit to 1
//@param m - the matrix holding the bit
//@param r - the row of the bit
//@param c - the col of the bit
void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    //get the index of the byte that the bit we want lays in
    uint32_t colByte = c / 8;
    //get the byte
    uint8_t byte = m->mat[r][colByte];

    //get the index of the bit we want in that byte
    c = c % 8;
    //make a byte with a 1 shifted c spots in (put a 1 at spot c)
    uint8_t mask = 1 << c;
    //set the byte in the ADT to the union of 'byte' and 'mask'
    m->mat[r][colByte] = byte | mask;
}

//sets a bit to 0
//@param m - the matrix holding the bit
//@param r - the row of the bit
//@param c - the col of the bit
void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    //get the index of the byte that the bit we want lays in
    uint32_t colByte = c / 8;
    //get the byte
    uint8_t byte = m->mat[r][colByte];

    //get the index of the bit we want in that byte
    c = c % 8;
    //make a byte with all 1s except one 0 at spot c
    uint8_t mask = ~(1 << c);
    // multiply the 'mask' and 'byte' bytes together so that all bits that
    // aren't in spot c stay the same while spot c turns to a 0
    m->mat[r][colByte] = byte & mask;
}

//gets a bit
//@param m - the matrix holding the bit
//@param r - the row of the bit
//@param c - the col of the bit
//@return - the value of the bit
uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    //get the index of the byte that the bit we want lays in
    uint32_t colByte = c / 8;
    //get the byte
    uint8_t byte = m->mat[r][colByte];
    //get the index of the bit we want in that byte
    c = c % 8;
    //make a byte with a 1 shifted c spots in (put a 1 at spot c)
    uint8_t mask = 1 << c;
    // multiply the 'mask' and 'byte' bytes together so that all bits that
    // aren't in spot c must be 0s
    uint8_t result = byte & mask;
    //move the bit to the least significant bit spot in the byte
    result = result >> c;

    return result;
}

//prints the matrix
//@param m - the matrix
void bm_print(BitMatrix *m) {
    for (uint32_t r = 0; r < m->rows; r++) {
        printf("[");
        for (uint32_t c = 0; c < m->cols; c++) {
            printf("%d ", bm_get_bit(m, r, c));
        }
        printf("]\n");
    }
}
