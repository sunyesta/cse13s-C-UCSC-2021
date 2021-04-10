#include "quick.h"

#include "stack.h"
#include "tools.h"

#include <stdio.h>
#include <stdlib.h>

void allocation_error(Stack **stack) {
    fprintf(stderr, "error allocating space\n");
    if (*stack != NULL) {
        stack_delete(stack);
    }
    exit(1);
}

// sorts a
int64_t partition(uint32_t *A, uint32_t lo, uint32_t hi) {

    //select midpoint of lo and hi as pivot
    uint32_t pivot_index = lo + ((hi - lo) / 2);
    uint32_t pivot = A[pivot_index];

    //l and r will be fixed to equal lo and hi inside the loop
    int l = lo - 1;
    int r = hi + 1;

    while (l < r) {
        //must increment l & r each itteration in case A[l] == A[r] == pivot
        l += 1;
        r -= 1;

        //find the first value from the left that's greater than the pivot
        while (st_is_smaller(A[l], pivot)) {
            l += 1;
        }
        //find the first value from the right that's less than the pivot
        while (st_is_smaller(pivot, A[r])) {
            r -= 1;
        }

        //flip the values if they are on the wrong sides of the pivot
        //
        //	if all values to the right of the pivot are greater than the pivot, r <p
        //	if all values to the left of the pivot are less than the pivot, l >p
        //	so r<l if nothing needs swapping
        if (l < r) {
            st_swap(A, l, r);
        }
    }

    //return the index of where the numbers greater than or equal to the partition start
    //
    //everything might not all be sorted around the index of the pivot
    //but we return r so that we get a division btwn numbers > or < the partition
    return r;
}

void quick_sort(uint32_t *A, uint32_t n) {
    Stack *stack = stack_create();
    if (stack == NULL) {
        allocation_error(&stack);
    }
    //pushes lowest and highest indexes in array to stack
    int64_t left = 0;
    int64_t right = n - 1;
    stack_push(stack, left);
    stack_push(stack, right);

    //until there are no more partitions to split, sort them
    while (stack_empty(stack) == false) {
        //take the upper and lower bounds for the partition out of the array
        int64_t hi = 0;
        int64_t lo = 0;
        if (stack_pop(stack, &hi) == false || stack_pop(stack, &lo) == false) {
            allocation_error(&stack);
        }

        //sort array inside range and get a new partition
        int64_t p = partition(A, lo, hi);

        //if the range above the partition is greater than 1, add it to the
        //stack to that we can sort it later
        if ((p + 1) < hi) {
            if (stack_push(stack, p + 1) == false || stack_push(stack, hi) == false) {
                allocation_error(&stack);
            }
        }

        //if the range below the partition is greater than 1, add it to the
        //stack to that we can sort it later
        if (lo < p) {
            if (stack_push(stack, lo) == false || stack_push(stack, p) == false) {
                allocation_error(&stack);
            }
        }
    }

    stack_delete(&stack);
}
