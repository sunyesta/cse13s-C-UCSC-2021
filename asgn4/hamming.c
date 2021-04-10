#include "hamming.h"

#include "bm.h"

#include <inttypes.h>
#include <stdio.h>

static BitMatrix *generator;
static BitMatrix *parity;

static int lookup[16];

// initializes all the global variables in hamming.c
// @return - HAM_ERR if creating the matricies fails, else HAM_OK
ham_rc ham_init(void) {

    const int rows = 4;
    const int cols = 8;

    //show if there was an error with calloc when creating the matricies
    if ((generator = bm_create(rows, cols)) == NULL || (parity = bm_create(cols, rows)) == NULL) {

        return HAM_ERR;
    }

    int half = cols / 2;

    //generator matrix:
    // 1 0 0 0  0 1 1 1
    // 0 1 0 0  1 0 1 1
    // 0 0 1 0  1 1 0 1
    // 0 0 0 1  1 1 1 0

    //sets 1st half of generator to have 1's down the diagonal
    for (int i = 0; i < rows; i++) {
        bm_set_bit(generator, i, i);
    }

    //sets 2nd half of generator to have 1's everywhere except the diagonal
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < half; c++) {
            //skip diagonal
            if (r == c) {
                continue;
            }
            bm_set_bit(generator, r, c + half);
        }
    }

    //parity matrix:
    // 0 1 1 1
    // 1 0 1 0
    // 1 1 0 1
    // 1 1 1 0
    //
    // 1 0 0 0
    // 0 1 0 0
    // 0 0 1 0
    // 0 0 0 1

    //copies 2nd half of generator matrix to upper half of parity matrix
    for (int r = 0; r < half; r++) {
        for (int c = 0; c < half; c++) {
            if (bm_get_bit(generator, r, c + half) == 1) {
                bm_set_bit(parity, c, r);
            }
        }
    }

    //copies 1st half of generator matrix to lower half of parity matrix
    for (int r = 0; r < rows; r++) {
        for (int c = half; c < cols; c++) {
            if (bm_get_bit(generator, r, c - half) == 1) {
                bm_set_bit(parity, c, r);
            }
        }
    }

    //initialize lookup[16]
    lookup[0] = -1;
    lookup[1] = 4;
    lookup[2] = 5;
    lookup[3] = -1;
    lookup[4] = 6;
    lookup[5] = -1;
    lookup[6] = -1;
    lookup[7] = 3;
    lookup[8] = 7;
    lookup[9] = -1;
    lookup[10] = -1;
    lookup[11] = 2;
    lookup[12] = -1;
    lookup[13] = 1;
    lookup[14] = 0;
    lookup[15] = -1;

    return HAM_OK;
}

// frees the memory taken by the matricies
void ham_destroy(void) {
    bm_delete(&generator);
    bm_delete(&parity);
}

// gets a bit from a byte
// @param byte - the byte
// @param c - the column of the bit
// @return - the bit from byte at col c
uint8_t get_bit(uint8_t byte, uint32_t c) {
    if (c >= 8) {
        fprintf(stdout, "indexing bad");
    }
    //make a byte with a 1 shifted c spots in (put a 1 at spot c)
    uint8_t mask = 1 << (c);
    // multiply the 'mask' and 'byte' bytes together so that all bits that
    // aren't in spot c must be 0s
    uint8_t result = byte & mask;
    //move the bit to the least significant bit spot in the byte
    result = result >> c;

    return result;
}

// sets a bit in a byte to 1
// @param byte - pointer to the byte you want to change
// @param c - column of the bit in the byte
void set_bit(uint8_t *byte, uint32_t c) {

    //check if index is in range of a byte
    if (c >= 8) {
        fprintf(stdout, "indexing bad");
    }

    //make a byte with a 1 shifted c spots in (put a 1 at spot c)
    uint8_t mask = 1 << c;
    //set the byte in the ADT to the union of 'byte' and 'mask'
    *byte = *byte | mask;
}

// sets a bit in a byte to 0
// @param byte - pointer to the byte you want to change
// @param c - column of the bit in the byte
void clr_bit(uint8_t *byte, uint32_t c) {

    //check if index is in range of a byte
    if (c >= 8) {
        fprintf(stdout, "indexing bad");
    }

    //make a byte with all 1s except one 0 at spot c
    uint8_t mask = ~(1 << c);
    // multiply the 'mask' and 'byte' bytes together so that all bits that
    // aren't in spot c stay the same while spot c turns to a 0
    *byte = *byte & mask;
}

// DEBUGGING FUNCTION
// prints bits in a byte
// @param byte - the byte
void print_bits(uint8_t byte) {
    printf("[");
    for (uint32_t c = 0; c < 8; c++) {
        printf("%d ", get_bit(byte, 7 - c));
    }
    printf("]\n");
}

// performes (bit vector)*(BitMatrix) where a byte is taken as a bit vector
// @param a - the byte
// @param b - pointer to the BitMatrix
// @return - byte equivalent to (bit vector)*(BitMatrix)
uint8_t multiply_mats(uint8_t a, BitMatrix *b) {
    const uint32_t ROWS_IN_A = 1;

    //result byte after multiplication process
    uint8_t result = 0;

    //itterates through all the rows in the byte which is 1
    //keeping the outside loop for clarity
    for (uint32_t a_row = 0; a_row < ROWS_IN_A; a_row++) {

        //itterates through all the cols of b
        for (uint32_t b_col = 0; b_col < bm_cols(b); b_col++) {

            //multiplies each col of a in its indexed row
            //by each row of b in its indexed col
            //and adds the results together
            int sum = 0;
            for (uint32_t i = 0; i < bm_rows(b); i++) {

                sum += get_bit(a, i) * bm_get_bit(b, i, b_col);
            }
            //puts sets sum to 0 or 1
            sum = sum % 2;

            //sets the bit in the result
            //clr_bit is unneeded bc result starts as 0000 0000
            if (sum == 1) {
                set_bit(&result, b_col);
            }
        }
    }
    return result;
}

// encodes a nibble of data
// @param data - data to encode
// @param code - a pointer to the byte the encoded text should be stored
// @return - HAM_ERR if init was not performed before or code == NULL, else HAM_OK
ham_rc ham_encode(uint8_t data, uint8_t *code) {

    //checks if init was called before and if code pointer was valid
    if (generator == NULL || code == NULL) {
        return HAM_ERR;
    }
    //encodes by multiplying data with the generator matrix
    *code = multiply_mats(data, generator);
    return HAM_OK;
}

// decodes an encoded nibble given the coded byte
// @param code - the coded byte
// @param code - a pointer to the nibble the decoded text should be stored
// @return - HAM_ERR	if init was not performed before or data == NULL
//			 HAM_OK		if byte has no error syndrome
//			 HAM_ERR_OK if the byte had an error syndrome but was corrected
//			 HAM_ERR	if the byte had an error syndrome but was unable to be corrected
ham_rc ham_decode(uint8_t code, uint8_t *data) {

    //checks if init was called before and if data pointer was valid
    if (parity == NULL || data == NULL) {
        return HAM_ERR;
    }

    //set data to code, assumes no bit flip is needed
    *data = code;

    //multiply code with parity matrix to check for errors in bits
    uint8_t error = multiply_mats(code, parity);

    //no error means no bit needs to be flipped
    if (error == 0) {
        return HAM_OK;
    }

    //find the error bit
    int bit_to_flip = lookup[error];

    //flip the error bit
    if (bit_to_flip != -1) {

        if (get_bit(*data, bit_to_flip) == 1) {
            clr_bit(data, bit_to_flip);
        } else {
            set_bit(data, bit_to_flip);
        }

        return HAM_ERR_OK;
        //no bit to flip found
    } else {
        return HAM_ERR;
    }
}
