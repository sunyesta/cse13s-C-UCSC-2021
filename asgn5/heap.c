#include "heap.h"

#include "tools.h"

#include <stdbool.h>

int max_child(uint32_t *A, int first, int last) {
    int left = 2 * first;
    int right = left + 1;

    //if the right child is larger than the other child,
    //return the right child as the max_child
    if ((right <= last) && st_is_smaller(A[left - 1], A[right - 1])) {
        return right;
    } else {
        return left;
    }
}

//fixes the heap
void fix_heap(uint32_t *A, int first, int last) {
    int found = false;
    int parent = first;
    int great = max_child(A, first, last);

    while ((parent <= (last / 2)) && (found == false)) {
        st_compares++;
        if (A[parent - 1] < A[great - 1]) {
            st_swap(A, parent - 1, great - 1);
            parent = great;
            great = max_child(A, parent, last);
        } else {
            found = true;
        }
    }
}

//builds the heap by rearranging the elements in the array
void build_heap(uint32_t *A, int first, int last) {
    for (int parent = last / 2; parent > first - 1; parent -= 1) {
        fix_heap(A, parent, last);
    }
}

void heap_sort(uint32_t *A, uint32_t n) {
    int first = 1;
    int last = n;

    //fix array arrangment
    build_heap(A, first, last);

    //sort the array by extracting elements from the heap
    for (int leaf = last; leaf > first; leaf--) {
        //move root A[0] to the correct spot in the array A[leaf-1]
        st_swap(A, first - 1, leaf - 1);
        //fix the heap so that it is arranged correctly
        fix_heap(A, first, leaf - 1);
    }
}
