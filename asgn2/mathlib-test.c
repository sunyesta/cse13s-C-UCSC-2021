#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "asctel"

//Prints the header for the function outputs
//@param *type - pointer to string that indicates the kind of function to be used
void printHeader(char *type) {
    printf("  %-12s %-16s %-14s %-12s\n", "x", type, "Library", "Difference");
    printf("  %-12s %-16s %-14s %-12s\n", "-", "----", "-------", "----------");
    return;
}

//Prints the output of the functions in the correct format
//@param input - input of functions
//@param mine - output of our function
//@param library - output of the <math.h>'s library function
void printInFormat(double input, double mine, double library) {
    printf(" %7.4lf %16.8lf %16.8lf %16.10lf\n", input, mine, library, mine - library);
    return;
}

//If there is an error with the inputs this function should be called
//It prints the options the user has for inputs, stops the program, and if there
//is an invalid input, it prints that input as well
//@param *directory - pointer to argv[0] should only be passed in
void inputError(char *directory) {
    fprintf(stderr, "Program usage: %s -[asctel]\n", directory);
    puts("  -a           runs all tests (sin, cos, tan, exp, log)");
    puts("  -s           runs sin tests");
    puts("  -c           runs cos tests");
    puts("  -t           runs tan tests");
    puts("  -e           runs exp tests");
    puts("  -l           runs log tests");
    exit(1);
}

//Gets user inputs and calls the corresponding functions
//@param argc - size of argument list
//@param **argv - the argument list
int main(int argc, char **argv) {
    int opt = 0;
    bool do_sin, do_cos, do_tan, do_exp, do_log;

    //terminates program with error if no inputs were given
    if (argc == 1) {
        inputError(argv[0]);
    }

    //itterates though list of arguments
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        //getOPT returns -1 when the argument list is exausted
        switch (opt) {
        case 'a':
            do_sin = true;
            do_cos = true;
            do_tan = true;
            do_exp = true;
            do_log = true;
            break;
        case 's': do_sin = true; break;
        case 'c': do_cos = true; break;
        case 't': do_tan = true; break;
        case 'e': do_exp = true; break;
        case 'l': do_log = true; break;
        default: inputError(argv[0]);
        }
    }

    //compares outputs of Sin(x) and sin(x) from [-2pi,2pi]
    if (do_sin) {
        printHeader("Sin");
        double threshold = 2 * M_PI;
        for (float i = -threshold; i <= threshold; i += .1) {
            printInFormat(i, Sin(i), sin(i));
        }
    }

    //compares outputs of Cos(x) and cos(x) from [-2pi,2pi]
    if (do_cos) {
        printHeader("Cos");
        double threshold = 2 * M_PI;
        for (float i = -threshold; i <= threshold; i += .1) {
            printInFormat(i, Cos(i), cos(i));
        }
    }

    //compares outputs of Tan(x) and tan(x) from [-pi/3,pi/3]
    if (do_tan) {
        printHeader("Tan");
        double threshold = M_PI / 3;
        for (float i = -threshold; i <= threshold; i += .1) {
            printInFormat(i, Tan(i), tan(i));
        }
    }

    //compares outputs of Exp(x) and exp(x) from [1,10]
    if (do_exp) {
        printHeader("Exp");

        for (float i = 1; i < 10; i += .1) {
            printInFormat(i, Exp(i), exp(i));
        }
    }

    //compares outputs of Log(x) and log(x) from [1,10]
    if (do_log) {
        printHeader("Log");
        for (float i = 1; i < 10; i += .1) {
            printInFormat(i, Log(i), log(i));
        }
    }

    return 0;
}
