// ADT for a bloom filter
// Should be used alongside hashtabe to check if an oldspeak has the possibiliy
// of being in the hash table
//
// bf_insert() should be always called alongside ht_insert() if a bloomfilter
// is attached to a hashtable
#include "bf.h"

#include "bv.h"
#include "speck.h"

#include <assert.h>
#include <stdlib.h>

struct BloomFilter {
    uint64_t primary[2]; //primary hash function salt
    uint64_t secondary[2]; //secondary hash function salt
    uint64_t tertiary[2]; //tertiary hash function salt
    BitVector *filter;
};

// the constructor for a bloom filter
// @param size - denotes the number of indices, or bits, that the bloom
//				 filter can index up to
// @return - the created bloom filter if creation was successful
//			 NULL if creation was not successful
BloomFilter *bf_create(uint32_t size) {
    //if size is 0, then you can't allocate space
    if (size == 0) {
        return NULL;
    }

    //dynamically allocate space for the new bloom filter container
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf == NULL) {
        return NULL;
    }

    //set primary salt
    bf->primary[0] = 0x02d232593fbe42ff;
    bf->primary[1] = 0x3775cfbf0794f152;

    //set secondary salt
    bf->secondary[0] = 0xc1706bc17ececc04;
    bf->secondary[1] = 0xe9820aa4d2b8261a;

    //set tertiary salt
    bf->tertiary[0] = 0xd37b01df0ae8f8d0;
    bf->tertiary[1] = 0x911d454886ca7cf7;

    //dynamically allocate space for the bit vector inside the bloom filter
    bf->filter = bv_create(size);
    if (bf->filter == NULL) {
        free(bf);
        bf = NULL;
    }

    return bf;
}

// frees the memory of the bloom filter
// @param bf - the bloom filter
void bf_delete(BloomFilter **bf) {
    //makes sure bf and *bf are not NULL
    assert(bf && *bf);

    bv_delete(&((*bf)->filter));
    free(*bf);

    bf = NULL;
}

// returns the bloom filter's size
// @param ht - the bloom filter
uint32_t bf_length(BloomFilter *bf) {
    //makes sure bf is not NULL
    assert(bf);

    return bv_length(bf->filter);
}

// gets the index of the hash value of an oldspeak string
// @param bf - the hash table we are using to find the index
// @param oldspeak - the oldspeak string we are hashing
// @param salt - the array of salts used to hash
// @return - the index of the hash value of an oldspeak string
uint32_t bf_get_hash_index(BloomFilter *bf, uint64_t salt[], char *oldspeak) {
    //makes sure bf is not NULL
    assert(bf);
    //makes sure oldspeak is not NULL
    assert(oldspeak);

    return hash(salt, oldspeak) % bf_length(bf);
}

// Takes oldspeak and inserts it into the Bloom filter.
// @param bf - the bloom filter
// @param oldspeak - the oldspeak being inserted
void bf_insert(BloomFilter *bf, char *oldspeak) {
    bv_set_bit(bf->filter, bf_get_hash_index(bf, bf->primary, oldspeak));
    bv_set_bit(bf->filter, bf_get_hash_index(bf, bf->secondary, oldspeak));
    bv_set_bit(bf->filter, bf_get_hash_index(bf, bf->tertiary, oldspeak));
}

// checks if an oldspeak could have been inserted in the bloom filter
// @param bf - the bloom filter
// @param oldspeak - the oldspeak being inserted
// @return - true if the value could have been inserted in the bloom filter
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    //makes sure bf is not NULL
    assert(bf);

    //checks if all the hash indexes of the oldspeak are true
    return bv_get_bit(bf->filter, bf_get_hash_index(bf, bf->primary, oldspeak))
           && bv_get_bit(bf->filter, bf_get_hash_index(bf, bf->secondary, oldspeak))
           && bv_get_bit(bf->filter, bf_get_hash_index(bf, bf->tertiary, oldspeak));
}

// prints the bloom filter
// @param bf - the bloom filter being printed
void bf_print(BloomFilter *bf) {
    //makes sure bf is not NULL
    assert(bf);

    bv_print(bf->filter);
}
