#ifndef LOGIC_H
#define LOGIC_H

#include "board.h"


enum turn {
    BLACK_NEXT,
    WHITE_NEXT
};

typedef enum turn turn;


enum outcome {
    BLACK_WIN,
    WHITE_WIN,
    DRAW,
    IN_PROGRESS
};

typedef enum outcome outcome;


enum direction {
    CW, CCW
};

typedef enum direction direction;


struct game {
    unsigned int run;
    board* b;
    turn next;
};

typedef struct game game;

/* new_game: creates a game of run and board of given width, height, and type */
game* new_game(unsigned int run, unsigned int width, unsigned int height,
               enum type type);

/* game_free: frees a dynamically allocated game */
void game_free(game* g);

/* drop: helper for implementing gravity on pieces */
void drop(game* g, unsigned int col, cell color);

/* drop_piece: drops a piece according to the player's turn */
int drop_piece(game* g, unsigned int col);

/* twist: rotates the board in direction d */
void twist(game* g, direction d);

/* check_horizontal: helper that iterates through a row to determine an 
 * outcome */
int check_horizontal(game* g, cell color);

/* check_vertical: helper that iterates through a column to determine an
 * outcome */
int check_vertical(game* g, cell color);

/* check_diagonal1: helper that iterates through the matrix to determine a
 * diagonal win (negative slope diagonal)  */
int check_diagonal1(game* g, cell color);

/* check_diagonal2: helper that iterates through the matrix to determine a
 * diagonal win (positive slope diagonal)  */
int check_diagonal1(game* g, cell color);

/* check_win: helper to determine if a player won */
int check_win(game* g, cell color);

/* game_outcome: determines the outcome of a game */
outcome game_outcome(game* g);

#endif /* LOGIC_H */
