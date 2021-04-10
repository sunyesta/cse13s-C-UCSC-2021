// ADT for an array of bytes - used to store true and false arrays in an
// efficient way

#include "bv.h"

#include <assert.h> //used for assert()
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
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

// constructor for a bit vector
// @param length - the length of the bit vector
BitVector *bv_create(uint32_t length) {
    //if size is 0, then you can't allocate space
    if (length == 0) {
        return NULL;
    }

    //dynamically allocate space for the new bit vector container
    BitVector *bv = (BitVector *) calloc(1, sizeof(BitVector));
    if (bv == NULL) {
        return NULL;
    }

    //set length of the bit vector
    bv->length = length;

    //dynamically allocate space for the bit vector
    bv->vector = (uint8_t *) (calloc(bytes(length), sizeof(uint8_t)));
    if (bv->vector == NULL) {
        bv_delete(&bv);
    }

    return bv;
}

// free's the memory allocated towards a bit vector
// @param bv - the bit vector
void bv_delete(BitVector **bv) {
    //make sure bv and *bv are not NULL
    assert(bv && *bv);

    free((*bv)->vector);
    free(*bv);

    *bv = NULL;
}

// gets the length of a bit vector
// @param bv - the bit vector
// @return - the length of the bit vector
uint32_t bv_length(BitVector *bv) {
    //make sure bv is not NULL
    assert(bv);

    return bv->length;
}

// sets a bit to 1 in a bit vector
// @param bv - the bit vector being changed
// @param i - the index of the bit
void bv_set_bit(BitVector *bv, uint32_t i) {
    //make sure bv is not NULL
    assert(bv);
    //make sure i is inside scope of the array, no need to check if i<=0 bc i is unsigned
    assert(i < bv->length);

    //get the index of the byte that the bit we want lays in
    uint32_t byte_containing_i = i / 8;
    //get the byte
    uint8_t byte = bv->vector[byte_containing_i];
    //get the index of the bit we want in that byte
    i = i % 8;
    //make a byte with a 1 shifted i spots in (put a 1 at spot i)
    uint8_t mask = 1 << i;
    //set the byte in the ADT to the union of 'byte' and 'mask'
    bv->vector[byte_containing_i] = byte | mask;
}

// sets a bit to 0 in a bit vector
// @param bv - the bit vector being changed
// @param i - the index of the bit
void bv_clr_bit(BitVector *bv, uint32_t i) {
    //make sure bv is not NULL
    assert(bv);
    //make sure i is inside scope of the array, no need to check if i<=0 bc i is unsigned
    assert(i < bv->length);

    //get the index of the byte that the bit we want lays in
    uint32_t byte_containing_i = i / 8;
    //get the byte
    uint8_t byte = bv->vector[byte_containing_i];
    //get the index of the bit we want in that byte
    i = i % 8;
    //make a byte with all 1s except one 0 at spot i
    uint8_t mask = ~(1 << i);
    // multiply the 'mask' and 'byte' bytes together so that all bits that
    // aren't in spot i stay the same while spot i turns to a 0
    bv->vector[byte_containing_i] = byte & mask;
}

// gets a bit in a bit vector
// @param bv - the bit vector being changed
// @param i - the index of the bit
// @return - the value of the bit
uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
    //make sure bv is not NULL
    assert(bv);
    //make sure i is inside scope of the array, no need to check if i<=0 bc i is unsigned
    assert(i < bv->length);

    //get the index of the byte that the bit we want lays in
    uint32_t byte_containing_i = i / 8;
    //get the byte
    uint8_t byte = bv->vector[byte_containing_i];
    //get the index of the bit we want in that byte
    i = i % 8;
    //make a byte with a 1 shifted i spots in (put a 1 at spot i)
    uint8_t mask = 1 << i;
    //multiply the 'mask' and 'byte' bytes together so that all bits that
    //aren't in spot i must be 0s
    uint8_t result = byte & mask;
    //move the bit to the least significant bit spot in the byte
    result = result >> i;

    return result;
}

// prints a bit vector
// @param bv - the bit vector being printed
void bv_print(BitVector *bv) {
    //make sure bv is not NULL
    assert(bv);

    printf("[");
    //itterates through the bit vector
    for (uint32_t i = 0; i < bv->length; i++) {
        printf("%d", bv_get_bit(bv, i));
    }
    printf("]\n");
}
