#include "bf.h"
#include "hash.h"
#include "ll.h"
#include "node.h"
#include "parser.h"

#include <ctype.h>
#include <errno.h> //used for checking if files are valid
#include <regex.h> //regex
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //getopt()

#define OPTIONS           "h:f:m"
#define DEFAULT_HT_SIZE   10000
#define DEFAULT_BF_SIZE   1048576 //2^20
#define MAX_STRING_LENGTH 1024

#define WORD "[A-Za-z0-9]+(('|-)[A-Za-z0-9]+)*"

// makes an uppercase string to lowercase
// @param string - the string being converted to lowercase
void lowercase(char *string) {
    //iterate through all characters in the string
    for (int i = 0; string[i] != '\0'; i++) {
        //convert each character to it's lowercase version
        string[i] = tolower(string[i]);
    }
}

// prints error message for failing to allocate memory
void allocation_failed() {
    fprintf(stderr, "failed to allocate memory\n");
}

int main(int argc, char **argv) {
    uint32_t ht_size = DEFAULT_HT_SIZE;
    uint32_t bf_size = DEFAULT_BF_SIZE;
    bool mtf = false;

    //parses the commandline options
    int opt;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            //size specifies that the hash table will have size entries
            //(the default will be 10000).
            ht_size = atoi(optarg);
            break;
        case 'f':
            //size specifies that the Bloom filter will have size entries
            //(the default will be 2^20 ).
            bf_size = atoi(optarg);
            break;
        case 'm':
            //enables the move-to-front rule
            mtf = true;
            break;
        default:
            puts("Usage: ./banhammer [-m] [-h size] [-f size]");
            puts("   -m          Enable move-to-front rule.");
            puts("   -h size     Set hash table size.");
            puts("   -f size     Set Bloom filter size.");

            exit(1);
        }
    }

    //creates the hash table to store the badspeak words
    HashTable *ht = ht_create(ht_size, mtf);
    if (ht == NULL) {
        allocation_failed();
        exit(1);
    }

    //creates the bloom filter
    BloomFilter *bf = bf_create(bf_size);
    if (bf == NULL) {
        ht_delete(&ht);
        allocation_failed();
        exit(1);
    }

    //------ reading files ------

    //opens the badspeak.txt file
    FILE *badspeak_txt = fopen("./badspeak.txt", "r");
    if (badspeak_txt == NULL || errno == EINVAL) {
        fprintf(stderr, "error getting file\n");

        ht_delete(&ht);
        bf_delete(&bf);

        exit(1);
    }

    //opens the newspeak.txt file
    FILE *newspeak_txt = fopen("./newspeak.txt", "r");
    if (newspeak_txt == NULL || errno == EINVAL) {
        fprintf(stderr, "error getting file\n");

        ht_delete(&ht);
        bf_delete(&bf);

        fclose(badspeak_txt);

        exit(1);
    }

    //read in badspeak words from badspeak.txt
    char bad_word[MAX_STRING_LENGTH];
    // 1023 = MAX_STRING_LENGTH - 1 so that there is space for the null at the end
    while (fscanf(badspeak_txt, "%1023s", bad_word) != EOF) {
        bf_insert(bf, bad_word);
        ht_insert(ht, bad_word, NULL);
    }

    //read in badspeak and newspeak words from newspeak.txt
    char new_word[MAX_STRING_LENGTH];
    while (fscanf(newspeak_txt, "%1023s %1023s", bad_word, new_word) != EOF) {
        bf_insert(bf, bad_word);
        ht_insert(ht, bad_word, new_word);
    }

    //close files once we are done reading from them
    fclose(badspeak_txt);
    fclose(newspeak_txt);

    //------ create linked lists to hold bad words and fixable words ------

    LinkedList *bad_words = ll_create(mtf);
    if (bad_words == NULL) {
        allocation_failed();
        ht_delete(&ht);
        bf_delete(&bf);
        exit(1);
    }

    LinkedList *fixable_words = ll_create(mtf);
    if (fixable_words == NULL) {
        ht_delete(&ht);
        bf_delete(&bf);

        ll_delete(&bad_words);

        allocation_failed();
        exit(1);
    }

    //------ read user input ------

    //compile regex
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        ht_delete(&ht);
        bf_delete(&bf);

        ll_delete(&bad_words);
        ll_delete(&fixable_words);

        perror("regcomp");
        exit(1);
    }

    //use regex to parse stdin
    char *word = NULL;
    while ((word = next_word(stdin, &re)) != NULL) {
        //make word into lowercase so that it can match the words in the files
        lowercase(word);

        //if word is not in bloomfilter, word is not in hashtable so go to next word
        if (bf_probe(bf, word) == false) {
            continue;
        }

        Node *n = ht_lookup(ht, word);
        //if word is not in hashtable, go to next word
        if (n == NULL) {
            continue;
        }

        if (n->newspeak == NULL) {
            //if word has no translation, insert it into bad_words
            ll_insert(bad_words, n->oldspeak, n->newspeak);
        } else {
            //if word has a translation, insert it into fixable_words
            ll_insert(fixable_words, n->oldspeak, n->newspeak);
        }
    }

    //------ print output if needed ------

    if (ll_length(bad_words) > 0) {
        puts("Dear Comrade,\n");
        puts("You have chosen to use degenerate words that may cause hurt");
        puts("feelings or cause your comrades to think unpleasant thoughts.");
        puts("This is doubleplus bad. To correct your wrongthink and");
        puts("preserve community consensus we will be sending you to joycamp");
        puts("administered by Medellin's Miniluv. Beware of the hippos.\n");
        puts("Your errors:\n");
        ll_print(bad_words);
        if (ll_length(fixable_words) > 0) {
            puts("\nThink of these words on your vacation!\n");
            ll_print(fixable_words);
        }

    } else if (ll_length(fixable_words) > 0) {
        puts("Dear Comrade,\n");
        puts("Submitting your text helps to preserve feelings and prevent");
        puts("badthink. Some of the words that you used are not goodspeak.");
        puts("The list shows how to turn the oldspeak words into newspeak.\n");
        ll_print(fixable_words);
    }

    //------ free memory from the heap ------

    clear_words();
    regfree(&re);

    ht_delete(&ht);
    bf_delete(&bf);

    ll_delete(&bad_words);
    ll_delete(&fixable_words);

    return 0;
}
