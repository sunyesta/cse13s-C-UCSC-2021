#include "tools.h"

int st_moves = 0;
int st_compares = 0;

void st_reset() {
    st_moves = 0;
    st_compares = 0;
}

// swappes 2 indexes in an array
// @param A - the array being modified
// @param index1 - the 1st index
// @param index2 - the 2nd index
void st_swap(uint32_t *A, int index1, int index2) {
    uint32_t temp = A[index1];
    A[index1] = A[index2];
    A[index2] = temp;

    //st_swap needs to perform 3 moves to swap 2 indexes
    st_moves += 3;
}

bool st_is_smaller(int A, int B) {
    st_compares++;
    return A < B;
}
