#include "set.h"

#include <inttypes.h>

// @return - an empty set (0)
Set set_empty(void) {
    // msb 0000 0000 lsb
    return 0;
}

// determines if a spot in the Set is a 0 or 1
// @param s - the set
// @param x - the spot you want to index in the Set
// @return - true if the spot is set to 1
//			 false if the spot is set to 0
bool set_member(Set s, uint8_t x) {
    //gets the intersection of the set and an int with a 1 shifted x spots in
    //any number that isn't 0 will return true
    return s & (1 << (x % 8));
}

// sets the value of a spot in the Set to 1
// @param s - the set
// @param x - the spot you want to be set in the Set
// @return - the updated Set
Set set_insert(Set s, uint8_t x) {
    //gets the union of the set and an int with a 1 shifted x spots in
    return s | (1 << (x % 8));
}

// sets the value of a spot in the Set to 0
// @param s - the set
// @param x - the spot you want to be set in the Set
// @return - the updated Set
Set set_remove(Set s, uint8_t x) {
    //gets the intersection of a set with all 1s except at spot x and the Set
    return s & (~(1 << (x % 8)));
}

// gets a set containing the elements that are common to both sets (the intersection)
// @param s - the 1st set
// @param t - the 2nd set
// @return - the intersetion of 2 sets
Set set_intersect(Set s, Set t) {
    return s & t;
}

// gets a set containing all the elements in both sets (the union)
// @param s - the 1st set
// @param t - the 2nd set
// @return - the union of the 2 sets
Set set_union(Set s, Set t) {
    return s | t;
}

// gets the complement of a set
// i.e. 1s turn to 0s and 0s turn to 1s
// @param s - the set
// @return - the updated Set
Set set_complement(Set s) {
    return ~(s);
}

// gets the elements of set s which are not in set t (the difference)
// @param s - the 1st set
// @param t - the 2nd set
// @return - the difference set
Set set_difference(Set s, Set t) {
    return s & (~t);
}
