// ADT for hash table - good for finding values in an array with close to O(1) time

#include "hash.h"

#include "speck.h"

#include <assert.h> //used for assert()
#include <stdio.h>
#include <stdlib.h>

struct HashTable {
    uint64_t salt[2]; //salts of hashtable used for hashing
    uint32_t size; //size of hashtable
    bool mtf; //if true, a node will move to the front whenever it is indexed
    LinkedList **lists; //array of linked lists
};

// the constructor for a hash table
// @param size - denotes the number of indices, or linked lists, that the hash
//				 table can index up to
// @param mtf - specifies for a node to be moved to the front if it is found
//				using ht_lookup()
// @return - the created hashtable if creation was successful
//			 NULL if creation was not successful
HashTable *ht_create(uint32_t size, bool mtf) {
    //if size is 0, then you can't allocate space
    if (size == 0) {
        return NULL;
    }

    //dynamically allocates memory for a hashtable
    HashTable *ht = (HashTable *) calloc(1, sizeof(HashTable));
    if (ht == NULL) {
        return NULL;
    }

    //sets the salts
    ht->salt[0] = 0x85ae998311115ae3; //Il nome della rosa
    ht->salt[1] = 0xb6fac2ae33a40089;

    ht->size = size;
    ht->mtf = mtf;

    //allocates space in every spot in the array for a linked list pointer
    ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
    if (ht->lists == NULL) {
        free(ht);
        return NULL;
    }

    return ht;
}

// frees the memory of the hashtable
// @param ht - the hashtable
void ht_delete(HashTable **ht) {
    //makes sure ht and *ht are not NULL
    assert(ht && *ht);

    //itterates through array of linked list pointers
    for (uint32_t i = 0; i < (*ht)->size; i++) {
        LinkedList *cur = ((*ht)->lists[i]);
        if (cur != NULL) {
            ll_delete(&(cur));
        }
    }

    free((*ht)->lists);
    free(*ht);

    *ht = NULL;

    return;
}

// returns the hash table’s size
// @param ht - the hashtable
uint32_t ht_size(HashTable *ht) {
    //makes sure ht is not NULL
    assert(ht);

    return ht->size;
}

// gets the index of the hash value of an oldspeak string
// @param ht - the hash table we are using to find the index
// @param oldspeak - the oldspeak string we are hashing
// @return - the index of the hash value of an oldspeak string
uint32_t ht_get_hash_index(HashTable *ht, char *oldspeak) {
    //makes sure ht is not NULL
    assert(ht);
    //makes sure oldspeak is not NULL
    assert(oldspeak);

    return hash(ht->salt, oldspeak) % ht->size;
}

// searches for an entry, a node, in the hash table that contains oldspeak
// @param ht - the hashtable being searched
// @param oldspeak - the oldspeak being searched for
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    //makes sure ht is not NULL
    assert(ht);
    //makes sure oldspeak is not NULL
    assert(oldspeak);

    //get the index the oldspeak string hashes to
    uint32_t hash_index = ht_get_hash_index(ht, oldspeak);

    //create a shortcut to the list we are looking in
    LinkedList *list = ht->lists[hash_index];

    //if there is no list at this index, the value does not exist in the table
    if (list == NULL) {
        return NULL;
    }

    //check if the value is inside the list
    return ll_lookup(list, oldspeak);
}

// inserts the specified oldspeak and its corresponding newspeak
// translation into the hash table
// @param ht - the hash table that oldspeak and newspeak are being inserted into
// @param oldspeak - the oldspeak string
// @param newspeak - the newspeak string
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    //makes sure ht is not NULL
    assert(ht);
    //makes sure oldspeak is not NULL
    assert(oldspeak);

    //get the index that the oldspeak string hashes to
    uint32_t hash_index = ht_get_hash_index(ht, oldspeak);

    //if there is no linked list at that index, create one
    if (ht->lists[hash_index] == NULL) {

        //create a new linked list
        LinkedList *newList = ll_create(ht->mtf);
        if (newList == NULL) {
            return;
        }

        //insert it into the hash table
        ht->lists[hash_index] = newList;
    }

    //insert a new node with the passed in values into the list
    ll_insert(ht->lists[hash_index], oldspeak, newspeak);

    //should be checking if ll_insert() failed but returning a bool is out of the
    //scope of this assignment

    return;
}

// prints a hashtable
// @param ht - the table being printed
void ht_print(HashTable *ht) {
    //makes sure ht is not NULL
    assert(ht);

    //print the header
    printf("hashtable: \n");

    //itterate through the lists
    for (uint32_t i = 0; i < ht->size; i++) {
        //print the current index of the list
        printf("  index %d:\n", i);

        LinkedList *cur = ht->lists[i];
        //if no linked list exists at this index, print NULL
        if (cur == NULL) {
            printf("NULL\n");

            //print the linked list
        } else {
            ll_print(cur);
        }

        printf("\n");
    }

    return;
}
