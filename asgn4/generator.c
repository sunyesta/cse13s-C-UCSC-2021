#include <errno.h> //used for checking if files are valid
#include <inttypes.h> //different int types library
#include <stdio.h>
#include <stdlib.h> //used for exit()
#include <unistd.h> //used for getopt()

//file status libraries
#include "bm.h"
#include "hamming.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "i:o:"

//returns the high nibble of a byte
uint8_t high_nibble(uint8_t n) {
    //240 dec -> 11110000 bin
    //makes all numbers in lower nibble 0, and shifts bytes to the right by 4
    //Notes: making bytes in lower nibble to 0 shouldn't matter, but its safer
    return (n & 240) >> 4;
}
//returns the low nibble of a byte
uint8_t low_nibble(uint8_t n) {
    //15 dec -> 00001111 bin
    //makes all numbers in the upper nibble 0
    return n & 15;
}

int main(int argc, char **argv) {
    FILE *infile = stdin;
    FILE *outfile = stdout;

    int opt;
    //reads through the user's inputs
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            //Specify the input file to read
            infile = fopen(optarg, "r");
            break;
        case 'o':
            //Specify the output file to be written
            outfile = fopen(optarg, "w");
            break;
        default: exit(1); //if there is an invalid input, exit the program
        }
    }

    //checks to see if infile is not a valid file
    if (infile == NULL || errno == EINVAL) {
        fprintf(stdout, "invalid input file\n");
        exit(1);
    }

    //make output file permission same as input file permission
    struct stat buf;
    //get permission status of infile
    fstat(fileno(infile), &buf);
    //set permission status of outfile to infile's permission status
    fchmod(fileno(outfile), buf.st_mode);

    //initialize the generator and parity matricies
    if (ham_init() == HAM_ERR) {
        fprintf(stdout, "error allocating memory\n");
        exit(1);
    }

    //loops through text and encodes it to the outfile
    int ch;
    while (((ch = fgetc(infile)) != EOF)) {

        //seperate byte into nibbles
        uint8_t lower = low_nibble(ch);
        uint8_t higher = high_nibble(ch);

        //generate codes
        uint8_t lower_code;
        uint8_t higher_code;
        if ((ham_encode(lower, &lower_code) == HAM_ERR)
            || (ham_encode(higher, &higher_code) == HAM_ERR)) {
            fprintf(stdout, "ham_init() not ran or empty pointer\n");
            exit(1);
        }

        //output code
        fputc(lower_code, outfile);
        fputc(higher_code, outfile);
    }

    ham_destroy();

    fclose(infile);
    fclose(outfile);

    return 0;
}
