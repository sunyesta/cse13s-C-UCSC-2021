#include "tools.h"

#include <assert.h>
#include <stdio.h>

//------------------- misc -------------------

// gets the bit length of a code
// @param code - the code
// @return - the bit length of the code
uint8_t bitlen(uint16_t code) {
    uint8_t len = 0;
    //shifts the bits in code to the right by 1 until code = 0
    while (code > 0) {
        code = code >> 1;
        len++;
    }
    return len;
}

// gets the amount of bytes needed for an amount of bits
// @param bits - the amount of bits
// @return - the amount of bytes needed for an amount of bits
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

//------------------- 8 bit functions -------------------

// gets a bit from a byte
// @param word - the uint16_t being indexed
// @param index - the index of the bit
// @return - the bit at the index
uint8_t bt_8_get_bit(uint8_t byte, uint8_t index) {
    //make sure that the index is in the range of the amount of bits in a byte
    assert(index < 8);

    //make a byte with a 1 shifted 'index' spots in (put a 1 at spot 'index')
    uint8_t mask = 1 << index;
    // multiply the 'mask' and 'byte' bytes together so that all bits that
    // aren't in spot 'index' must be 0s
    uint8_t result = byte & mask;
    //move the bit to the least significant bit spot in the byte
    result = result >> index;

    return result;
}

// sets a bit to 1 in byte
// @param word - pointer to the byte being changed
// @param i - the index of the bit int the byte being changed
void bt_8_set_bit(uint8_t *byte, uint8_t index) {
    //make sure that the index is in the range of the amount of bits in a byte
    assert(index < 8);

    //make a byte with a 1 shifted 'index' spots in (put a 1 at spot 'index')
    uint8_t mask = 1 << index;
    //set the byte to be the union of 'byte' and 'mask'
    *byte = *byte | mask;
}

// sets a bit to 0 in a byte
// @param word - pointer to the byte being changed
// @param index - index of the bit in the byte
void bt_8_clr_bit(uint8_t *byte, uint8_t index) {
    //make sure that the index is in the range of the amount of bits in a byte
    assert(index < 8);

    //make a byte with all 1s except one 0 at spot 'index'
    uint8_t mask = ~(1 << index);
    // multiply the 'mask' and 'byte' bytes together so that all bits that
    // aren't in spot 'index' stay the same while spot 'index' turns to a 0
    *byte = *byte & mask;
}

//------------------- 16 bit functions -------------------

// gets a bit from a word
// @param word - the uint16_t being indexed
// @param index - the index of the bit
// @return - the bit at the index
uint16_t bt_16_get_bit(uint16_t word, uint8_t index) {
    //make sure that the index is in the range of the amount of bits in a word
    assert(index < 16);

    //make a word with a 1 shifted 'index' spots in (put a 1 at spot 'index')
    uint16_t mask = 1 << index;
    // multiply the 'mask' and 'word' words together so that all bits that
    // aren't in spot 'index' must be 0s
    uint16_t result = word & mask;
    //move the bit to the least significant bit spot in the word
    result = result >> index;

    return result;
}

// sets a bit to 1 in word
// @param word - pointer to the word being changed
// @param i - the index of the bit int the word being changed
void bt_16_set_bit(uint16_t *word, uint8_t index) {
    assert(index < 16);

    //make a word with a 1 shifted 'index' spots in (put a 1 at spot 'index')
    uint16_t mask = 1 << index;
    //set the word to be the union of 'word' and 'mask'
    *word = *word | mask;
}

// sets a bit to 0 in a word
// @param word - pointer to the word being changed
// @param index - index of the bit in the word
void bt_16_clr_bit(uint16_t *word, uint8_t index) {
    //make sure that the index is in the range of the amount of bits in a word
    assert(index < 16);

    //make a word with all 1s except one 0 at spot 'index'
    uint16_t mask = ~(1 << index);
    // multiply the 'mask' and 'word' words together so that all bits that
    // aren't in spot 'index' stay the same while spot 'index' turns to a 0
    *word = *word & mask;
}

//------------------- buffer functions -------------------

//gets a bit from an array of bytes
//@param m - the matrix holding the bit
//@param r - the row of the bit
//@param c - the col of the bit
//@return - the value of the bit
uint8_t bt_buf_get_bit(uint8_t *bytes, uint32_t bit_index) {
    return bt_8_get_bit(bytes[bit_index / 8], bit_index % 8);
}

// sets a bit to 1 in an array of bytes
// @param word - pointer to the word being changed
// @param i - the index of the bit int the word being changed
void bt_buf_set_bit(uint8_t *bytes, uint32_t bit_index) {
    bt_8_set_bit(&bytes[bit_index / 8], bit_index % 8);
}

//sets a bit to 0 in an array of bytes
//@param m - the matrix holding the bit
//@param r - the row of the bit
//@param c - the col of the bit
void bt_buf_clr_bit(uint8_t *bytes, uint32_t bit_index) {
    bt_8_clr_bit(&bytes[bit_index / 8], bit_index % 8);
}

//------------------- print functions -------------------

void bt_8_print(uint8_t byte) {
    printf("[");
    for (int i = 0; i < 8; i++) {
        printf("%d", bt_8_get_bit(byte, i));
    }
    printf("]\n");
}

void bt_16_print(uint16_t word) {
    printf("[");
    for (int i = 0; i < 16; i++) {
        printf("%d", bt_16_get_bit(word, i));
    }
    printf("]\n");
}

void bt_buf_print(uint8_t *bytes, uint32_t bitlength) {
    printf("[");
    for (uint32_t i = 0; i < bitlength; i++) {
        printf("%d", bt_buf_get_bit(bytes, i));
    }
    printf("]\n");
}
