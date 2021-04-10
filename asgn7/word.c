#include "word.h"

#include "code.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// the constructor for a word
// @param syms - the array of symbols in the word
// @param len - the length of syms
// @return - the created word if creation was successful
//			 NULL if creation was not successful
Word *word_create(uint8_t *syms, uint32_t len) {
    //allocates memory for a word in the heap
    Word *w = (Word *) calloc(1, sizeof(Word));
    if (w == NULL) {
        return NULL;
    }

    w->len = len;

    //allocates memory for w->syms in the heap
    w->syms = (uint8_t *) calloc(len, sizeof(uint8_t));
    if (w->syms == NULL) {
        free(w);
        return NULL;
    }

    //copy syms into w->syms
    for (uint32_t i = 0; i < len; i++) {
        w->syms[i] = syms[i];
    }
    return w;
}

// creates a new word with a symbol appended to the end of the passed in word
// @param w - the word
// @param sym - the symbol being appended
// @return - the new word
Word *word_append_sym(Word *w, uint8_t sym) {
    assert(w);

    // create new word
    Word *new_word = calloc(1, sizeof(Word));
    if (new_word == NULL) {
        return NULL;
    }

    //allocate enough size for the old word + enough size for the new sym
    new_word->len = w->len + 1;
    new_word->syms = (uint8_t *) calloc(new_word->len, sizeof(uint8_t));
    if (new_word->syms == NULL) {
        free(new_word);
        return NULL;
    }

    // copy the syms from 'w' to 'new_word'
    for (uint32_t i = 0; i < w->len; i++) {
        new_word->syms[i] = w->syms[i];
    }

    // set the last spot of 'new_word' to 'sym'
    new_word->syms[new_word->len - 1] = sym;

    return new_word;
}

// frees the memory allocated to a word
// @param w - the word
void word_delete(Word *w) {
    assert(w);

    free(w->syms);
    free(w);
}

// creates an array of words with an empty word initialized at index EMPTY_CODE
// @return - the word table (array of words)
WordTable *wt_create(void) {
    WordTable *wt = (WordTable *) calloc(MAX_CODE, sizeof(WordTable));
    if (wt == NULL) {
        return NULL;
    }

    wt[EMPTY_CODE] = word_create(NULL, 0);

    return wt;
}

// deletes all the words in the table
// @param wt - the word table
void wt_delete_words(WordTable *wt) {
    assert(wt);

    //itterate through all the words in the table
    for (int i = 0; i < MAX_CODE; i++) {
        //if the word exists, delete it
        if (wt[i] != NULL) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
}

// sets all the words in a word table to NULL except the first word which is an empty word
// @param wt - the word table
void wt_reset(WordTable *wt) {
    assert(wt);

    wt_delete_words(wt);
    //creates the empty word in the first spot of the table
    wt[EMPTY_CODE] = word_create(NULL, 0);
}

// frees the memory allocated to a word table and its words
// @param wt - the word table
void wt_delete(WordTable *wt) {
    assert(wt);

    wt_delete_words(wt);
    free(wt);
}

// DEBUGGING FUNCTION
// prints a word
// @param w - the word
void word_print(Word *w) {
    assert(w);

    for (uint32_t i = 0; i < w->len; i++) {
        printf("%c", w->syms[i]);
    }
    printf("\n");
}

// DEBUGGING FUNCTION
// prints a word table
// @param wt - the word table
// @param len - the length of the word table
void wt_print(WordTable *wt, uint32_t len) {
    assert(wt);

    printf("[\n");
    for (uint32_t i = 0; i < len; i++) {
        if (wt[i] != NULL) {
            word_print(wt[i]);
        } else {
            printf("(NULL)\n");
        }
    }
    printf("]\n");
}
