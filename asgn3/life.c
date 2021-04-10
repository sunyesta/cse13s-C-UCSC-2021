#include "universe.h" //to use my universe.c library

#include <errno.h> //used for checking if files are valid
#include <ncurses.h> //for creating the screen
#include <stdbool.h> //used for bool
#include <stdio.h> //standard input output
#include <stdlib.h> //used for exit() and atoi()
#include <unistd.h> //for usleep()

#define ROW   0
#define DELAY 50000

#define ALIVE true
#define DEAD  false

// prints error and exits the program
// should be used for invalid inputs
void malformedInput() {
    fprintf(stderr, "Malformed input.\n");
    exit(1);
}

// calculates the next generation of universe A
// new gen inside Universe *a
// old gen inside Universe *b
// @param pointerToA - the pointer of the point to Universe A
// @param pointerToB - the pointer of the point to Universe B
void calculateNextGen(Universe **pointerToA, Universe **pointerToB) {

    Universe *a = *pointerToA;
    Universe *b = *pointerToB;

    //loops though every slot in 'a' and writes next gen in 'b'
    for (int r = 0; r < uv_rows(a); r++) {
        for (int c = 0; c < uv_cols(a); c++) {
            int neighbors = uv_census(a, r, c);

            //Any live cell with two or three live neighbors survives.
            if (uv_get_cell(a, r, c) == ALIVE && (neighbors == 3 || neighbors == 2)) {
                uv_live_cell(b, r, c);

                //Any dead cell with exactly three live neighbors becomes a live cell.
            } else if (uv_get_cell(a, r, c) == DEAD && neighbors == 3) {
                uv_live_cell(b, r, c);

                //All other cells die, either due to loneliness or overcrowding.
            } else {
                uv_dead_cell(b, r, c);
            }
        }
    }

    //swaps pointers
    Universe *temp = a;
    *pointerToA = b;
    *pointerToB = temp;

    return;
}

// Reads the flags
// prints output
// calls void calculateNextGen()
int main(int argc, char **argv) {
    bool toroidal = false;
    bool silentNC = false;
    int genCount = 100;
    FILE *inFile = stdin;
    FILE *outFile = stdout;
    int opt;

    //reads through the user's inputs
    while ((opt = getopt(argc, argv, "tsn:i:o:")) != -1) {
        switch (opt) {
        case 't':
            //Specify that the Game of Life is to be played on a toroidal universe.
            toroidal = true;
            break;
        case 's':
            //Silence ncurses.
            //Enabling this option means that nothing should be displayed by ncurses.
            silentNC = true;
            break;
        case 'n':
            // Specify the number of generations that the universe goes through.
            //The default number of generations is 100.
            genCount = atoi(optarg);
            break;
        case 'i':
            //Specify the input file to read in order to populate the universe.
            //By default the input should be stdin.
            inFile = fopen(optarg, "r");
            break;
        case 'o':
            //Specify the output file to print the final state of the universe to.
            //By default the output should be stdout.
            outFile = fopen(optarg, "w");
            break;
        default: exit(1); //if there is an invalid input, exit the program
        }
    }

    //checks to see in inFile is not a valid file
    if (inFile == NULL || errno == EINVAL) {
        malformedInput();
    }

    //counts rows and cols to be put in the universe (first line of input)
    int rows;
    int cols;

    //error handling for invalid inputs
    int scan = fscanf(inFile, "%d%d", &rows, &cols);
    if (scan == 0 || scan == EOF) {
        malformedInput();
    }

    //creates a universe and populates it from the file
    Universe *a = uv_create(rows, cols, toroidal);
    bool success = uv_populate(a, inFile);

    if (success == false) {
        malformedInput();
    }

    Universe *b = uv_create(rows, cols, toroidal);

    //calculates the universe and prints it each time
    if (silentNC == false) {

        initscr(); //initialize the screen
        curs_set(FALSE); //Hide the cursor

        //loops until we have calculated the specified number of generations
        for (int gen = 0; gen < genCount; gen++) {

            calculateNextGen(&a, &b);
            clear(); //clears the screen

            //itterates through the slots in the universe
            for (int r = 0; r < uv_rows(a); r++) {
                for (int c = 0; c < uv_cols(a); c++) {

                    //if slot is alive, print "o"
                    if (uv_get_cell(a, r, c) == ALIVE) {
                        mvprintw(r, c, "o");

                        //if slot is dead, print " "
                    } else {
                        mvprintw(r, c, " ");
                    }
                }
            }
            refresh(); //Refresh the window
            usleep(DELAY); //sleep for 50000 microsrconds
        }

        endwin(); //close the screen

        //only print the next gen
    } else {

        //loops until we have calculated the specified number of generations
        for (int gen = 0; gen < genCount; gen++) {
            calculateNextGen(&a, &b);
        }
    }

    //prints the final generation in the specified output file
    uv_print(a, outFile);

    //frees the allocated memory in the universes
    uv_delete(a);
    uv_delete(b);

    //closes the files
    fclose(inFile);
    fclose(outFile);

    return 0;
}
