#include "shell.h"

#include "gaps.h"
#include "tools.h"

#include <inttypes.h>

void shell_sort(uint32_t *A, uint32_t n) {

    //itterates through the gap sizes
    for (uint32_t g = 0; g < GAPS; g++) {

        //gets the current gap size
        uint32_t gap = gaps[g];

        //itterates through the array
        for (uint32_t i = gap; i < n; i++) {

            //temp is the base number we are using to compare
            uint32_t temp = A[i];
            uint32_t j = i;

            // keep swapping the indexes with temp as the base until
            // temp > than the val behind it
            while ((j >= gap) && st_is_smaller(temp, A[j - gap])) {
                //swaps temp with val behind it
                st_swap(A, j, j - gap);
                //temp is now in index j-gap, so we need to
                //make the base index into j-gap
                j = j - gap;
            }
        }
    }
}
