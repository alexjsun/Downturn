#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logic.h"

/* print_outcome: prints the outcome of a game */
void print_outcome(outcome o){
    if (o == BLACK_WIN){
        printf("BLACK_WIN\n");
    } else if (o == WHITE_WIN){
        printf("WHITE_WIN\n");
    } else if (o == DRAW){
        printf("DRAW\n");
    } else {
        printf("IN_PROGRESS\n");
    }
}

/* convert_input: converts a input char into an unsigned integer (column) */
unsigned int convert_input(char input){
    if (input >= '0' && input <= '9'){
        return input - 48;
    } else if (input >= 'A' && input <= 'Z'){
        return input - 55;
    } else if (input >= 'a' && input <= 'z'){
        return input - 61;
    } else {
        return 100; /* so it returns an invalid input for valid_input */
    }
}

/* col_full: determines if a column is full */
int col_full(game* g, unsigned int col){
    unsigned i;
    for (i = 0; i < g->b->height; i++){
        pos p = make_pos(i, col);
        if (board_get(g->b, p) == EMPTY){
            return 0;
        }
    }
    return 1;
}

/* valid_input: determines if the input is valid */
int valid_input(game* g, char input){
    if (input == '<' || input == '>'){
        return 1;
    }
    unsigned col = convert_input(input);
    if (col >= 0 && col < g->b->width && !col_full(g, col)){
        return 1;
    } else {
        return 0;
    }
}

/* read_input: reads the inputted scanf value and interprets it as a move */
void read_input(game* g, char input){
    if (input == '<'){
        twist(g, CCW);
    } else if (input == '>'){
        twist(g, CW);
    }
    unsigned int col = convert_input(input);
    if (col >= 0 && col < g->b->width){
        drop_piece(g, col);
    }
}

/* prompt: loop that alternates between players for their move and prints
 * the outcome if the game is over */
void prompt(game* g){
    unsigned i = 0;
    char move;
    while (game_outcome(g) == IN_PROGRESS){
        if (i % 2 == 0){
            g->next = BLACK_NEXT;
            printf("Black: ");
            scanf("%c%*c", &move);
            if (valid_input(g, move)){
                read_input(g, move);
                board_show(g->b);
                i++;
            } else {
                int m = convert_input(move);
                if (m >= g->b->width){
                    printf("Out of bounds - please enter a valid move:\n");
                } else if (col_full(g, m)){
                    printf("Column full - please enter a different value:\n");
                } else {
                    printf("Invalid input - please enter a valid move:\n");
                }
            }
        } else {
            g->next = WHITE_NEXT;
            printf("White: ");
            scanf("%c%*c", &move);
            if (move == '~'){
                fprintf(stderr, "escape key: ~\n");
                exit(1);
            }
            if (valid_input(g, move)){
                read_input(g, move);
                board_show(g->b);
                i++;
            } else {
                int m = convert_input(move);
                if (m >= g->b->width){
                    printf("Out of bounds - please enter a valid move:\n");
                } else if (col_full(g, m)){
                    printf("Column full - please enter a different value:\n");
                } else {
                    printf("Invalid input - please enter a valid move:\n");
                }
            }
        }
    }
    print_outcome(game_outcome(g));
}

/* read_command: reads inputs of command line and builds the game */
game* read_command(int argc, char *argv[]){
    if (argc != 8){
        fprintf(stderr, "invalid number of arguments in command-line\n");
        exit(1);
    }
    unsigned int width = 0, height = 0, run = 0;
    enum type t = 2;
    unsigned i;
    for (i = 1; i < argc; i++){
        if (!strcmp("-w", argv[i])){
            width = atoi(argv[i + 1]);
            if (width > 62){
                fprintf(stderr, "read_command: too many columns\n");
                exit(1);
            }
            i++;
        } else if (!strcmp("-h", argv[i])){
            height = atoi(argv[i + 1]);
            if (height > 62){
                fprintf(stderr, "read_command: too many rows\n");
                exit(1);
            }
            i++;
        } else if (!strcmp("-r", argv[i])){
            run = atoi(argv[i + 1]);
            i++;
        } else if (!strcmp("-m", argv[i])){
            t = MATRIX;
        } else if (!strcmp("-b", argv[i])){
            t = BITS;
        } else {
            fprintf(stderr, "invalid command-line options\n");
            exit(1);
        }
    }
    if (height == 0){
        fprintf(stderr, "read_command: missing height parameter\n");
        exit(1);
    } else if (width == 0){
        fprintf(stderr, "read_command: missing width parameter\n");
        exit(1);
    } else if (run == 0){
        fprintf(stderr, "read_command: missing run parameter\n");
        exit(1);
    } else if (t == 2){
        fprintf(stderr, "read_command: missing type parameter\n");
        exit(1);
    } else {
        game* g = new_game(run, width, height, t);
        return g;
    }
}

int main(int argc, char *argv[]){
    game* g = read_command(argc, argv);
    board_show(g->b);
    prompt(g);
    return 0;
}
