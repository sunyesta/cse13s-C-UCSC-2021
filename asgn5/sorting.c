#include "bubble.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stack.h"
#include "tools.h"

#include <inttypes.h>
#include <stdbool.h> //bool
#include <stdio.h>
#include <stdlib.h> //used for exit() and atoi()
#include <unistd.h> //getopt()

#define OPTIONS "absqhr:n:p:"

#define DEFAULT_SEED       7092016
#define DEFAULT_ARRAYSIZE  100
#define DEFAULT_PRINTCOUNT 100
#define PRINT_COLS         5
typedef enum Sort {
    BUBBLE,
    SHELL,
    QUICK,
    HEAP,
} Sort;

// Fills array with sudo random numbers
// @param A - the array
// @param size - size of the array
// @param seed - the seed used to calculate the random numbers
void fill_rand(uint32_t *A, int size, uint64_t seed) {
    //reinitializes the random seed to that we generate the same array each time
    srandom(seed);

    //itterates through the array
    for (int i = 0; i < size; i++) {
        // 1073741823 is 30 1s in binary
        // we are bitmasking the random number to 30 bits
        A[i] = random() & 1073741823;
    }
}

// Prints the array
// @param A - the array
// @param B - the amount of items to print (Must be <= length of array)
void print_array(uint32_t *A, int print_count) {
    //itterates through the array
    for (int i = 0; i < print_count; i++) {
        printf("%13" PRIu32, A[i]);

        // if the current column is the size of PRINT_COLS
        // or the printing is done, then break line
        if (((i + 1) % PRINT_COLS) == 0 || i == print_count - 1) {
            printf("\n");
        }
    }
}

int main(int argc, char **argv) {
    Set enabledSorts = set_empty(); //Set of enabled sorts
    uint64_t seed = DEFAULT_SEED;
    uint32_t array_size = DEFAULT_ARRAYSIZE;
    uint32_t print_count = DEFAULT_PRINTCOUNT;

    //parses the commandline options
    int opt;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            // Employs all sorting algorithms.
            enabledSorts = set_empty();
            enabledSorts = set_complement(enabledSorts);
            break;
        case 'b':
            // Enables Bubble Sort.
            enabledSorts = set_insert(enabledSorts, BUBBLE);
            break;
        case 's':
            // Enables Shell Sort.
            enabledSorts = set_insert(enabledSorts, SHELL);
            break;
        case 'q':
            // Enables Quicksort.
            enabledSorts = set_insert(enabledSorts, QUICK);
            break;
        case 'h':
            // Enables Heapsort.
            enabledSorts = set_insert(enabledSorts, HEAP);
            break;
        case 'r':
            // Set the seed for randomization.
            seed = atoi(optarg);
            break;
        case 'n':
            // Set the array size.
            array_size = atoi(optarg);
            break;
        case 'p':
            // Number of elements to print from the array.
            print_count = atoi(optarg);
            break;
        }
    }

    if (array_size <= 0) {
        fprintf(stderr, "Can't sort an array with size 0 ¯\\_(シ)_/¯\n");
        exit(1);
    }

    //initializes the array and stores it in the heap where it has more space
    uint32_t *rand_nums = calloc(array_size, sizeof(uint32_t));

    //make sure there is enough space for the array to fit in memory
    if (rand_nums == NULL) {
        fprintf(stderr, "error allocating space for random number array");
        exit(1);
    }

    //fixes print_count to not be more than the array size
    if (array_size < print_count) {
        print_count = array_size;
    }

    //performs the activated sort operations
    //itterate through sort types
    for (Sort s = BUBBLE; s <= HEAP; s++) {

        //check if current sort is activated
        if (set_member(enabledSorts, s)) {

            //resets array so that it is not sorted
            fill_rand(rand_nums, array_size, seed);

            //resets the counters
            st_reset();

            //performs specified sort
            switch (s) {
            case BUBBLE:
                printf("Bubble Sort\n");
                bubble_sort(rand_nums, array_size);
                break;
            case SHELL:
                printf("Shell Sort\n");
                shell_sort(rand_nums, array_size);
                break;
            case QUICK:
                printf("Quick Sort\n");
                quick_sort(rand_nums, array_size);
                break;
            case HEAP:
                printf("Heap Sort\n");
                heap_sort(rand_nums, array_size);
                break;
            }
            printf("%d elements, %d moves, %d compares\n", array_size, st_moves, st_compares);

            print_array(rand_nums, print_count);
        }
    }

    free(rand_nums);

    return 0;
}
