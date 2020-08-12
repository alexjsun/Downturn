#include <stdlib.h>
#include <stdio.h>
#include "logic.h"

game* new_game(unsigned int run, unsigned int width, unsigned int height, 
               enum type type){
    if (run < 2 || (run > width && run > height)){
        fprintf(stderr, "new_game: invalid run\n");
        exit(1);
    }
    game* res = (game*) malloc(sizeof(game));
    res->run = run;
    res->b = board_new(width, height, type);
    res->next = BLACK_NEXT;
    return res;
}

void game_free(game* g){
    board_free(g->b);
    free(g);
}

/* drop: helper for implementing gravity on pieces */
void drop(game* g, unsigned int col, cell color){
    int i;
    for (i = g->b->height - 1; i >= 0; i--){
        pos p = make_pos(i, col);
        if (board_get(g->b, p) == EMPTY){
            board_set(g->b, p, color);
            break;
        }
    }
}

int drop_piece(game* g, unsigned int col){
    if (col > g->b->width - 1){
        fprintf(stderr, "drop_piece: invalid column\n");
        exit(1);
    }
    if (g->next == BLACK_NEXT){
        drop(g, col, BLACK);
        return 1;
    }
    if (g->next == WHITE_NEXT){
        drop(g, col, WHITE);
        return 1;
    }
    return 0;
}

void twist(game* g, direction d){
    board* tmp = g->b;
    board* res;
    res = board_new(g->b->height, g->b->width, g->b->type);
    g->b = res;
    unsigned i;
    int j;
    if (d == CW){
        for (i = 0; i < tmp->height; i++){
            for (j = tmp->width - 1; j >= 0; j--){
                pos p = make_pos(i, j);
                if (board_get(tmp, p) == BLACK){
                    drop(g, tmp->height - i - 1, BLACK);
                } else if (board_get(tmp, p) == WHITE){
                    drop(g, tmp->height - i - 1, WHITE);
                }
            }
        }
    } else {
        for (i = 0; i < tmp->height; i++){
            for (j = 0; j < tmp->width; j++){
                pos p = make_pos(i, j);
                if (board_get(tmp, p) == BLACK){
                    drop(g, i, BLACK);
                } else if (board_get(tmp, p) == WHITE){
                    drop(g, i, WHITE);
                }
            }
        }
    }
    board_free(tmp);
}

/* check_horizontal: helper that iterates through a row to determine an 
 * outcome */
int check_horizontal(game* g, cell color){
    unsigned i, j, res = 0;
    for (i = 0; i < g->b->height; i++){
        for (j = 0; j < g->b->width; j++){
            pos p = make_pos(i, j);
            if (board_get(g->b, p) == color){
                res++;
            } else {
                res = 0;
            }
            if (res == g->run){
                return 1;
            }
        }
        res = 0;
    }
    return 0;
}


/* check_vertical: helper that iterates through a column to determine an
 * outcome */
int check_vertical(game* g, cell color){
    unsigned i, j, res = 0;
    for (i = 0; i < g->b->width; i++){
        for (j = 0; j < g->b->height; j++){
            pos p = make_pos(j, i);
            if (board_get(g->b, p) == color){
                res++;
            } else {
                res = 0;
            }
            if (res == g->run){
                return 1;
            }
        }
        res = 0;
    }
    return 0;
}

/* check_diagonal1: helper that iterates through the matrix to determine a
 * diagonal win (negative slope diagonal)  */
int check_diagonal1(game* g, cell color){
    unsigned i, j, k, res = 0;
    for (k = 0; k < g->b->height; k++){
        for (i = k, j = 0; i < g->b->height && j < g->b->width; i++, j++){
            pos p = make_pos(i, j);
            if (board_get(g->b, p) == color){
                res++;
            } else {
                res = 0;
            }
            if (res == g->run){
                return 1;
            }
        }
        res = 0;
    }
    for (k = 1; k < g->b->width; k++){
        for (i = 0, j = k; i < g->b->height && j < g->b->width; i++, j++){
            pos p = make_pos(i, j);
            if (board_get(g->b, p) == color){
                res++;
            } else {
                res = 0;
            }
            if (res == g->run){
                return 1;
            }
        }
        res = 0;
    }
    return 0;
}

/* check_diagonal2: helper that iterates through the matrix to determine a
 * diagonal win (positive slope diagonal)  */
int check_diagonal2(game* g, cell color){
    int i, j, k;
    unsigned res = 0;
    for (k = 0; k < g->b->width; k++){
        for (i = k, j = g->b->width - 1; i < g->b->height && j >= 0; i++, j--){
            pos p = make_pos(i, j);
            if (board_get(g->b, p) == color){
                res++;
            } else {
                res = 0;
            }
            if (res == g->run){
                return 1;
            }
        }
        res = 0;
    }
    for (k = 0; k < g->b->width - 1; k++){
        for (i = 0, j = k; i < g->b->height && j < g->b->width - 1; i++, j--){
            pos p = make_pos(i, j);
            if (board_get(g->b, p) == color){
                res++;
            } else {
                res = 0;
            }
            if (res == g->run){
                return 1;
            }
        }
        res = 0;
    }
    return 0;
}

/* check_win: helper to determine outcome */
int check_win(game* g, cell color){
    return check_horizontal(g, color) || check_vertical(g, color) ||
           check_diagonal1(g, color) || check_diagonal2(g, color);
}

/* check_full: helper to determine if the board is full */
int check_full(game* g){
    unsigned i, j;
    for (i = 0; i < g->b->height; i++){
        for (j = 0; j < g->b->width; j++){
            pos p = make_pos(i, j);
            if (board_get(g->b, p) == EMPTY){
                return 0;
            }
        }
    }
    return 1;
}

outcome game_outcome(game* g){
    if ((check_win(g, BLACK) && check_win(g, WHITE)) || check_full(g)){
        return DRAW;
    } else if (check_win(g, BLACK)){
        return BLACK_WIN;
    } else if (check_win(g, WHITE)){
        return WHITE_WIN;
    } else {
        return IN_PROGRESS;
    }
}
