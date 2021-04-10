#include "stack.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h> //calloc stuff

void empty_pointers() {
    fprintf(stderr, "invalid empty pointer\n");
    exit(1);
}

struct Stack {
    uint32_t top;
    uint32_t capactiy;
    int64_t *items;
};

// creates a stack pointer for an empty stack
// @return the stack pointer
Stack *stack_create(void) {
    Stack *s = (Stack *) calloc(1, sizeof(Stack));

    s->top = 0;
    s->capactiy = MIN_CAPACITY; // 16

    // allocates space for 16 64-bit integers
    s->items = (int64_t *) calloc(s->capactiy, sizeof(int64_t));
    if (s->items == NULL) {
        free(s);
        return NULL;
    }

    return s;
}

// free's the stack and sets the pointer to NULL
// @param s - the pointer to the stack pointer
void stack_delete(Stack **s) {
    if (s == NULL || *s == NULL) {
        empty_pointers();
    }

    free((*s)->items);
    free(*s);
    *s = NULL;

    return;
}

// checks if the stack is empty
// @param s - the stack pointer
// @return true if stack is empty
bool stack_empty(Stack *s) {
    if (s == NULL) {
        empty_pointers();
    }

    return s->top == 0; // true if stack is empty
}

// pushes a value to the top of the stack
// @param s - the stack pointer
// @param x - the value to be pushed to the stack
bool stack_push(Stack *s, int64_t x) {
    if (s == NULL) {
        empty_pointers();
    }

    //checks if stack is full
    if (s->top == s->capactiy) {

        // doubles the capacity
        s->capactiy *= 2;

        // reallocates memory
        //	s->items = origional ptr
        //	2nd param = make space for capacity amount of int64_t bytes
        s->items = realloc(s->items, s->capactiy * sizeof(int64_t));

        if (s->items == NULL) {
            return false;
        }
    }

    //puts a val in the top slot in the array
    s->items[s->top] = x;

    //makes top point at next avalible position
    s->top += 1;

    return true;
}

// pops a value off the stack
// @param s - the stack pointer
// @param x - the pointer to where the popped item will be stored
bool stack_pop(Stack *s, int64_t *x) {
    if (s == NULL || x == NULL) {
        empty_pointers();
    }
    //if stack is empty, return false
    if (s->top == 0) {
        return false;
    }

    //makes top point to the highest filled slot
    s->top -= 1;

    //sets the value x pts to, to the thing in the top slot
    //
    //there is no need to set the top slot to 0 since we will just pretend
    //that it's empty now and overwrite it if we want to
    *x = s->items[s->top];

    return true;
}

// prints the stack
// @param s - the stack pointer
void stack_print(Stack *s) {
    if (s == NULL) {
        empty_pointers();
    }

    printf("[");
    for (uint32_t i = 0; i < s->top; i++) {
        printf("%" PRId64 ", ", s->items[i]);
    }
    printf("]\n");
    return;
}
