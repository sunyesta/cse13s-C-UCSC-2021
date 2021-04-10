// ADT for linked list - good for quickley inserting and reordering values
#include "ll.h"

#include <assert.h> //used for assert()
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //used for strcmp()

struct LinkedList {
    uint32_t length; //number of nodes in the linked list
    Node *head; //the head node of the linked list
    Node *tail; //the tail node of the linked list
    bool mtf; //if true, whenever a node is looked up, it will be moved to the front
};

// the constructor for a linked list
// @param mtf - true if you want ll_lookup() to also move the found node to the
//				head for quicker locating next time
// @return - the created linked list if creation was successful
//			 NULL if creation was not successful
LinkedList *ll_create(bool mtf) {
    //dynamically allocate space for the new linked list container
    LinkedList *ll = (LinkedList *) calloc(1, sizeof(LinkedList));
    if (ll == NULL) {
        return NULL;
    }

    //set length to 0
    ll->length = 0;

    //create sentinel nodes
    ll->head = node_create(NULL, NULL);
    if (ll->head == NULL) {
        free(ll);
        return NULL;
    }

    ll->tail = node_create(NULL, NULL);
    if (ll->tail == NULL) {
        free(ll->head);
        free(ll);
        return NULL;
    }

    //connect sentinel nodes
    ll->head->next = ll->tail;
    ll->tail->prev = ll->head;

    //set "move to front" variable
    ll->mtf = mtf;

    return ll;
}

// frees the allocated memory of the linked list
// @param ll - the linked list being deleted
void ll_delete(LinkedList **ll) {
    //makes sure ll and *ll are not NULL
    assert(ll && *ll);

    //make sure sentinal nodes are defined
    assert((*ll)->head);
    assert((*ll)->tail);

    //itterates through all the nodes btwn head and tail, including tail
    for (Node *cur = (*ll)->head->next; cur != NULL; cur = cur->next) {
        if (cur != NULL) {
            //deletes the previous node to cur
            node_delete(&(cur->prev));
        }
    }
    //free the tail node bc there is no NULL->prev
    node_delete(&((*ll)->tail));

    free(*ll);

    *ll = NULL;
    return;
}

// gets the length of the linked list, which is equivalent to the number of
// nodes in the linked list, not including the head and tail sentinel nodes.
// @param ll - the node being printed
// @return - the length of the linked list
uint32_t ll_length(LinkedList *ll) {
    //makes sure ll is not NULL
    assert(ll);

    return ll->length;
}

// moves a node to the head of a linked list
// @param ll - the linked list
// @param cur - the node being moved
void move_to_front(LinkedList *ll, Node *cur) {
    //makes sure ll is not NULL
    assert(ll);

    //unlink cur from linked list
    Node *prev = cur->prev;
    Node *next = cur->next;
    prev->next = next;
    next->prev = prev;

    //stick cur to front
    cur->next = ll->head->next;
    cur->prev = ll->head;

    //fix the head node so that it is connected to cur
    ll->head->next = cur;

    //fix the node that used to be directly behind head
    //but now it's behind cur
    Node *new_next = cur->next;
    new_next->prev = cur;

    //we don't change new_next->next

    return;
}

// searches for a node containing oldspeak
// @param ll - the linked list being searched
// @param oldspeak - the oldspeak string being searched for
// @return - the node which has its oldspeak equal to the passed in oldspeak string
Node *ll_lookup(LinkedList *ll, char *oldspeak) {
    //makes sure ll is not NULL
    assert(ll);

    //itterates through all the nodes btwn head and tail
    for (Node *cur = ll->head->next; cur != ll->tail; cur = cur->next) {

        //if a node matching to oldspeak is found, return it
        if (strcmp(cur->oldspeak, oldspeak) == 0) {

            //if "move to front" is enabled, move the found node to the front
            if (ll->mtf == true) {
                move_to_front(ll, cur);
            }

            return cur;
        }
    }

    //if no matching node was found, return NULL
    return NULL;
}

// Inserts a new node containing the specified oldspeak and newspeak
// into the linked list.
// @param ll - the node will be inserted into this linked list
// @param oldspeak - this is the oldspeak of the node
// @param newspeak - this is the newspeak of the node
void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
    //makes sure ll is not NULL
    assert(ll);

    //if the node is already present in the array list, don't do anything
    if (ll_lookup(ll, oldspeak) != NULL) {
        return;
    }

    //creates the new node
    Node *new = node_create(oldspeak, newspeak);
    if (new == NULL) {
        return;
    }

    //creates a shortcut to the head node for simplicity
    Node *head = ll->head;

    //fix new
    new->prev = head;
    new->next = head->next;

    //relinks the node that used to be right behind the head node
    head->next->prev = new;

    //links the new node to the head node
    head->next = new;

    //increments the size of the linked list
    ll->length++;

    return;
}

// prints the linked list
// @param ll - the linked list being printed
void ll_print(LinkedList *ll) {
    //makes sure ll is not NULL
    assert(ll != NULL);

    //itterates through all the nodes btwn head and tail
    for (Node *cur = ll->head->next; cur != ll->tail; cur = cur->next) {
        node_print(cur);
    }
    return;
}
