#include "trie.h"

#include "code.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// the constructor for a trie node
// @param code - code stored inside the trienode
// @return - the created trie node if creation was successful
//			 NULL if creation was not successful
TrieNode *trie_node_create(uint16_t code) {
    TrieNode *n = (TrieNode *) calloc(1, sizeof(TrieNode));
    if (n == NULL) {
        return NULL;
    }

    n->code = code;

    for (int i = 0; i < ALPHABET; i++) {
        n->children[i] = NULL;
    }

    return n;
}

// frees the memory allocated to a trie node
// @param n - the trie node
void trie_node_delete(TrieNode *n) {
    free(n);

    return;
}

// the constructor for a trie
// @return - the created trie if creation was successful
//			 NULL if creation was not successful
TrieNode *trie_create(void) {
    return trie_node_create(EMPTY_CODE);
}

// deletes all children nodes to a root node
// @param n - the root node
void trie_reset(TrieNode *root) {
    //itterate through all the children of the current node
    for (int i = 0; i < ALPHABET; i++) {
        TrieNode *child = root->children[i];
        if (child != NULL) {
            //delete each child's subtree
            trie_delete(child);
            root->children[i] = NULL;
        }
    }

    return;
}

// recursivley deletes a root node and its children
// @param n - the root node
void trie_delete(TrieNode *n) {
    assert(n);

    //itterate through all the children of the current node
    for (int i = 0; i < ALPHABET; i++) {
        TrieNode *child = n->children[i];
        if (child != NULL) {
            //delete each child's subtree
            trie_delete(child);
            n->children[i] = NULL;
        }
    }
    //delete the root node
    trie_node_delete(n);
    n = NULL;

    return;
}

// get a pointer to the child node representing sym
// @param n - the parent node
// @param sym - the symbol that we are getting the pointer of
// @return - the child node of n that represents sym
//			 NULL if no child node that represents sym exists
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    return n->children[sym];
}
