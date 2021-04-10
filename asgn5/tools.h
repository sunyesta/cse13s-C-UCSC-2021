#ifndef __SORTING_TOOLS_H__
#define __SORTING_TOOLS_H__

#include <stdbool.h>
#include <stdint.h>

int st_moves;
int st_compares;

void st_swap(uint32_t *Array, int index1, int index2);
bool st_is_smaller(int A, int B);

void st_reset();

#endif
