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

//returns the low nibble of a byte
uint8_t low_nibble(uint8_t n) {
    //15 dec -> 00001111 bin
    //makes all numbers in the upper nibble 0
    return n & 15;
}

// turns 2 nibbles into a byte
// @param byte1 - 1st byte
// @param byte2 - 2nd byte
uint8_t byte_from_nibble(uint8_t byte1, uint8_t byte2) {
    //get the lower nibble of the first byte
    byte1 = low_nibble(byte1);
    //move byte2 to the upper nibble
    byte2 = byte2 << 4;

    //return the union of the bytes
    return byte1 | byte2;
}

// updates error values
// @param status - error type
// @param uncorrected_errors - uncorected errors in main function
// @param corrected_errors - corrected errors in main function
ham_rc update(ham_rc status, int *uncorrected_errors, int *corrected_errors) {
    if (status == HAM_ERR) {
        *uncorrected_errors += 1;
    } else if (status == HAM_ERR_OK) {
        *corrected_errors += 1;
    }
    return status;
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

    //make output file permission same as input file permission
    struct stat buf;
    //get permission status of infile
    fstat(fileno(infile), &buf);
    //set permission status of outfile to infile's permission status
    fchmod(fileno(outfile), buf.st_mode);

    //checks to see if infile is not a valid file
    if (infile == NULL || errno == EINVAL) {
        fprintf(stdout, "invalid input file\n");
        exit(1);
    }

    //initialize the generator and parity matricies
    if (ham_init() == HAM_ERR) {
        fprintf(stdout, "error allocating memory\n");
    }

    //initialize tracker variables
    int total_bytes = 0;
    //uncorrected errors
    int uc_errors = 0;
    //corrected errors
    int c_errors = 0;

    //loops through encoded text and decodes it to the outfile
    //ch1 and ch2 must be ints bc EOF = -1
    int ch1;
    int ch2;
    while (((ch1 = fgetc(infile)) != EOF) && ((ch2 = fgetc(infile)) != EOF)) {
        total_bytes += 2;

        //generate text from codes
        uint8_t decoded_ch1;
        uint8_t decoded_ch2;
        update(ham_decode(ch1, &decoded_ch1), &uc_errors, &c_errors);
        update(ham_decode(ch2, &decoded_ch2), &uc_errors, &c_errors);

        //join nibbles
        uint8_t text = byte_from_nibble(decoded_ch1, decoded_ch2);

        //output result
        fputc(text, outfile);
    }

    fclose(infile);
    fclose(outfile);

    ham_destroy();

    fprintf(stderr, "Total bytes processed: %d\n", total_bytes);
    fprintf(stderr, "Uncorrected errors: %d\n", uc_errors);
    fprintf(stderr, "Corrected errors: %d\n", c_errors);
    fprintf(stderr, "Error rate: %f\n", (float) (uc_errors) / total_bytes);

    return 0;
}
