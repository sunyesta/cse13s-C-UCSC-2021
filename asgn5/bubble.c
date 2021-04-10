//#include <inttypes.h>
#include "bubble.h"

#include "tools.h"

#include <stdbool.h> //bool

void bubble_sort(uint32_t *A, uint32_t n) {

    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (uint32_t i = 1; i < n; i++) {
            //if the current val is smaller than the val behind it, swap them
            if (st_is_smaller(A[i], A[i - 1])) {
                st_swap(A, i, i - 1);
                swapped = true;
            }
        }
        n -= 1;
    }
}
