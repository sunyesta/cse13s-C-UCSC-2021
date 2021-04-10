#ifndef __BT_H__
#define __BT_H__

#include <stdint.h>

//------------------- misc -------------------

uint8_t bitlen(uint16_t code);

uint32_t bytes(uint32_t bits);

//------------------- 8 bit functions -------------------
uint8_t bt_8_get_bit(uint8_t byte, uint8_t index);

void bt_8_set_bit(uint8_t *byte, uint8_t index);

void bt_8_clr_bit(uint8_t *byte, uint8_t index);

//------------------- 16 bit functions -------------------
uint16_t bt_16_get_bit(uint16_t word, uint8_t index);

void bt_16_set_bit(uint16_t *word, uint8_t index);

void bt_16_clr_bit(uint16_t *word, uint8_t index);

//------------------- buffer functions -------------------
uint8_t bt_buf_get_bit(uint8_t *bytes, uint32_t bit_index);

void bt_buf_set_bit(uint8_t *bytes, uint32_t bit_index);

void bt_buf_clr_bit(uint8_t *bytes, uint32_t bit_index);

//------------------- buffer functions -------------------
void bt_8_print(uint8_t byte);

void bt_16_print(uint16_t word);

void bt_buf_print(uint8_t *bytes, uint32_t length);

#endif
