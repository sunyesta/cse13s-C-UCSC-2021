#include "names.h"

#include <inttypes.h> //fixed width-integers, printf specifiers
#include <stdio.h>
#include <stdlib.h>

uint32_t lives[sizeof(names) / sizeof(names[0])]; //lives defined up here for simplicity

// Returns a number between 0 and 5
uint8_t roll(void) {
    return random() % 6;
}

// Counts all the alive vampires
// @param playerCount - size of lives[]
uint8_t vampiresAlive(int playerCount) {
    int vampiresAlive = 0;
    for (int i = 0; i < playerCount; i++) {
        vampiresAlive += (lives[i] == 0) ? 0 : 1;
    }

    return vampiresAlive;
}

// gets the first living vampires in the lives array
// @param playerCount - size of the amount of players in the game
// @return - index of first living valmpire
uint8_t getFirstLivingVP(int playerCount) {
    for (int i = 0; i < playerCount; i++) {
        if (lives[i] > 0) {
            return i;
        }
    }
    //if there aren't any living vampires left, there is an error with the program
    fprintf(stderr, "No living vampires found\n");
    exit(1);
}

// returns the position of the player to the left
// this function is from the assignment instructions
// @param pindex - index of player you will use to find the player to the left
// @param playerCount - size of the amount of players in the game
// @return position of the player to the left
uint8_t getPlayerToLeft(int pindex, int playerCount) {
    return (pindex + playerCount - 1) % playerCount;
}

// returns the position of the player to the right
// this function is from the assignment instructions
// @param pindex - index of player you will use to find the player to the right
// @param playerCount - size of the amount of players in the game
// @return position of the player to the right
uint8_t getPlayerToRight(int pindex, int playerCount) {
    return (pindex + 1) % playerCount;
}

// Statment for simple printing of a player sparkling or ressurecting
// @param pindex - index of the player
void printState(int pindex) {
    if (lives[pindex] > 0) {
        printf(" sparkles %s", names[pindex]);
    } else {
        printf(" resurrects %s", names[pindex]);
    }
    return;
}

int main(void) {

    //ask for number of players
    uint8_t playerCount;
    printf("Number of players: ");

    //gets user input for player count and makes sure user input is valid
    if (scanf("%" SCNu8, &playerCount) != 1 || playerCount < 2
        || playerCount > sizeof(names) / sizeof(names[0])) {
        fprintf(stderr, "Invalid number of players.\n");
        exit(1);
    }

    //ask for a seed
    int64_t seed; //64 bit signed int
    printf("Random seed: ");

    if (scanf("%" SCNd64, &seed) != 1 || seed < 0 || seed > UINT32_MAX) {
        // If scanf doesn't return 1, it means that the users didn't input
        // a 64 bit signed int
        fprintf(stderr, "Invalid random seed.\n"); //prints an error message
        exit(1); //return 1; also works here
    }

    // changes the start point for the seed of the random() functions
    srandom(seed);

    //sets all lives to 3
    //only fills up needed spots for security and memory conservation
    for (int i = 0; i < playerCount; i++) {
        lives[i] = 3;
    }

    uint32_t round = 1;
    //loop for each round until 1 player is left alive
    while (vampiresAlive(playerCount) > 1) {

        uint8_t lowestRoll = 11; //lowest roll becomes above max roll number

        uint8_t lrvi;

        printf("Round %d\n", round);

        //iterate through players
        for (int i = 0; i < playerCount; i++) {
            if (lives[i] == 0) {
                continue;
            }

            //player rolls twice
            int roll1 = roll();
            int roll2 = roll();

            //prints what the player rolls
            printf(" - %s rolls %s...", names[i], rolls[roll1][roll2]);

            //checks if roll is lowest roll so far this round
            if (lowestRoll > roll1 + roll2) {
                lowestRoll = roll1 + roll2;
                lrvi = i;
            }

            //checks for midnight roll
            if (roll1 + roll2 == 10) {
                int leftPlayer = getPlayerToLeft(i, playerCount);
                int rightPlayer = getPlayerToRight(i, playerCount);

                //prints whether the vampires ressurect or sparkle
                printState(leftPlayer);
                printState(rightPlayer);

                //increments the lives of the vampires
                lives[leftPlayer]++;
                lives[rightPlayer]++;
            }

            printf("\n");
        }

        //takes a life from the lowest rolling vampire
        lives[lrvi]--;
        printf("%s is forced to eat garlic!\n", names[lrvi]);

        //print remaining lives of garlic victim
        switch (lives[lrvi]) {
        case 1: printf("%s has %d life remaining.\n", names[lrvi], lives[lrvi]); break;
        case 0: printf("%s has died.\n", names[lrvi]); break;
        default: printf("%s has %d lives remaining.\n", names[lrvi], lives[lrvi]); break;
        }

        round++;
    }

    //prints the winner!
    printf("%s wins the Garlic Game!\n", names[getFirstLivingVP(playerCount)]);

    return 0;
}
