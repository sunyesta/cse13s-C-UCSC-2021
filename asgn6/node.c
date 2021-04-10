// ADT for a node - made to be used inside of linked list
#include "node.h"

#include <assert.h> //used for assert()
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// copies a string to another string
// @param pointer - address of string that will be copied to
// @param string_to_cpy - string to copy from
// @return - true if copying the string was successful
bool string_cpy(char **pointer, char *string_to_cpy) {
    //makes sure the memory adress of the string is not NULL
    assert(pointer);

    //check if string needs to be copied
    if (string_to_cpy == NULL) {
        *pointer = NULL;

        //copies the string
    } else {
        //allocates space for the new string
        *pointer = (char *) malloc(strlen(string_to_cpy) * sizeof(char) + 1);

        //if allocating space failed, return false to show that it was not successful
        if (*pointer == NULL) {
            return false;
        }

        //copy the string character by character into *pointer
        strcpy(*pointer, string_to_cpy);
    }

    //copying the string was successful
    return true;
}

// the constructor for a node
// @param oldspeak - the oldspeak value of the node
// @param newspeak - the newspeak value of the node
// @return - the created node if creation was successful
//			 NULL if creation was not successful
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = calloc(1, sizeof(Node));

    //set n->oldspeak and n->newspeak
    if (string_cpy(&(n->oldspeak), oldspeak) == false) {
        free(n);
        return NULL;
    }
    if (string_cpy(&(n->newspeak), newspeak) == false) {
        free(n->oldspeak);
        free(n);
        return NULL;
    }

    n->next = NULL;
    n->prev = NULL;

    return n;
}

// free's the memory allocated in the heap for a node to delete it
// @param n - the node being deleted
void node_delete(Node **n) {
    //makes sure n and *n are not NULL
    assert(n && *n);

    free((*n)->oldspeak);
    free((*n)->newspeak);

    free(*n);
    *n = NULL;

    return;
}

// prints a node
// @param n - the node being printed
void node_print(Node *n) {
    //makes sure n is not NULL
    assert(n);

    //if the node has a newspeak, print both oldspeak and newspeak
    if (n->newspeak != NULL) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);

        //if the node doesn't have a newspeak, only print oldspeak
    } else {
        printf("%s\n", n->oldspeak);
    }

    return;
}
