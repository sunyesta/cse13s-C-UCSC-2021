#include "universe.h"

#include <stdbool.h>
#include <stdlib.h>

#define ALIVE true
#define DEAD  false

// @param rows - the amount of rows in the Universe's grid
// @param cols - the amount of cols in the Universe's grid
// @param grid - the grid of the universe
// @param toroidal - value of toroidal in the universe
struct Universe {
    int rows;
    int cols;
    bool **grid;
    bool toroidal;
};

// creates a pointer to a Universe
// @param rows - the amount of rows in the Universe's grid
// @param cols - the amount of cols in the Universe's grid
// @param toroidal - value of toroidal in the universe
// @return - pointer to the created Universe
struct Universe *uv_create(int rows, int cols, bool toroidal) {
    Universe *u = (Universe *) calloc(1, sizeof(Universe));

    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;

    u->grid = (bool **) calloc(rows, sizeof(bool *));

    //allocates memory for every column in the grid and puts a zero in every slot
    for (int i = 0; i < rows; i++) {
        u->grid[i] = (bool *) calloc(cols, sizeof(bool));
    }

    return u;
}

// frees the memory from a universe
// @param u - the pointer to the Universe
void uv_delete(Universe *u) {
    //frees every row of memory in the grid
    for (int r = 0; r < uv_rows(u); r++) {
        free(u->grid[r]);
    }
    //frees the grid's pointer and the Universe
    free(u->grid);
    free(u);

    return;
}

// Getter for a Universe’s rows
// @param u - the pointer to the Universe
// @return - the number of rows in the Universe
int uv_rows(Universe *u) {
    return u->rows;
}

// Getter for a Universe’s columns
// @param u - the pointer to the Universe
// @return - the number of rows in the Universe
int uv_cols(Universe *u) {
    return u->cols;
}

// sets a cell in a Universe to alive (true)
// @param u - the pointer to the Universe
// @param r - row of the cell
// @param c - column of the cell
void uv_live_cell(Universe *u, int r, int c) {

    u->grid[r][c] = ALIVE;
    return;
}

// sets a cell in a Universe to dead (false)
// @param u - the pointer to the Universe
// @param r - row of the cell
// @param c - column of the cell
void uv_dead_cell(Universe *u, int r, int c) {

    u->grid[r][c] = DEAD;
    return;
}

// getter for cell status (if out of bounds -> false)
// @param u - the pointer to the Universe
// @param r - row of the cell
// @param c - column of the cell
// @return - status of the cell (alive/true or dead/false)
bool uv_get_cell(Universe *u, int r, int c) {

    //if cell is out of bounds, cell can be read as dead
    if (r >= uv_rows(u) || r < 0 || c >= uv_cols(u) || c < 0) {
        return false;

        //returns value inside passed in cell
    } else {
        return u->grid[r][c];
    }
}

// populates a universe according to the specifications of the file
// @param u - the pointer to the Universe
// @param infile - the file that contains the coordinates of the alive cells
// @return - false if error / true if no error
bool uv_populate(Universe *u, FILE *infile) {

    int r;
    int c;

    while (true) {
        int scan = fscanf(infile, "%d%d", &r, &c);

        //stop loop if we reach EOF
        if (scan == EOF) {
            break;
        }

        //if scan doesn't return 2, then there was an error so return false
        if (scan != 2) {
            return false;
        }

        //if scanned values are out of bounds, return false
        if (r >= uv_rows(u) || r < 0 || c >= uv_cols(u) || c < 0) {
            return false;
        }
        uv_live_cell(u, r, c);
    }

    return true;
}

// calculates the amount of live neighbors to a cell in a toroidal or non-toroidal universe
// @param u - the pointer to the Universe
// @param r - row of the cell
// @param c - column of the cell
// @return - amount of live neighbors
int uv_census(Universe *u, int r, int c) {

    int aliveNeighbors = 0;

    //loop through all neighbors
    for (int nr = -1; nr <= 1; nr++) {
        for (int nc = -1; nc <= 1; nc++) {

            //skips itself
            if (nr == 0 && nc == 0) {
                continue;
            }

            //gets row and col of neighbor
            int rowOfNeighbor = r + nr;
            int colOfNeighbor = c + nc;

            //fixes value for toroidal if row or col is out of bounds
            if (u->toroidal == true) {
                rowOfNeighbor = (rowOfNeighbor + u->rows) % u->rows;
                colOfNeighbor = (colOfNeighbor + u->cols) % u->cols;
            }

            //increments aliveNeighbors if neighbor is alive
            if (uv_get_cell(u, rowOfNeighbor, colOfNeighbor) == ALIVE) {
                aliveNeighbors++;
            }
        }
    }

    return aliveNeighbors;
}

// prints the universe into a file
// @param u - the pointer to the Universe
// @param infile - the file that you want to universe printed in
void uv_print(Universe *u, FILE *outfile) {

    //loops through all slots of the Universe's grid
    for (int r = 0; r < u->rows; r++) {
        for (int c = 0; c < u->cols; c++) {

            //if the cell is alive, print "o"
            if (uv_get_cell(u, r, c) == ALIVE) {
                fprintf(outfile, "o");

                //if the cell is dead, print "."
            } else {
                fprintf(outfile, ".");
            }
        }

        //print a new line after every row
        fprintf(outfile, "\n");
    }

    return;
}
