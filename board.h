#ifndef BOARD_H
#define BOARD_H

#include "pos.h"


enum cell {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum cell cell;


union board_rep {
    enum cell** matrix;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    MATRIX, BITS
};


struct board {
    unsigned int width, height;
    enum type type;
    board_rep u;
};

typedef struct board board;

/* board_new: creates a new board of with given dimensions and type */
board* board_new(unsigned int width, unsigned int height, enum type type);

/* board_free: frees a dynamically allocated board */
void board_free(board* b);

/* print_labels: helper to print labels of matrix */
void print_labels(unsigned val);

/* board_show: prints the representation of the board into the console */
void board_show(board* b);

/* board_get: retrieves the value of the board at position p */
cell board_get(board* b, pos p);

/* board_set: sets a cell c at position p on the given board */
void board_set(board* b, pos p, cell c);

#endif /* BOARD_H */
