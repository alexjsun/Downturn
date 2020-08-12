#include <stdlib.h>
#include <stdio.h>
#include "board.h"

/* length: determines the length of an array for bit representation */
unsigned length(unsigned width, unsigned height){
    unsigned len;
    if (width * height % 16 != 0){
        len = ((width * height) / 16) + 1;
    } else {
        len = (width * height) / 16;
    }
    return len;
}

board* board_new(unsigned int width, unsigned int height, enum type type){
    if (width == 0 || height == 0){
        fprintf(stderr, "board_new: height/width cannot be 0\n");
        exit(1);
    }
    board* res = (board*) malloc(sizeof(board));
    if (res == NULL){
        fprintf(stderr, "board_new: malloc failed\n");
        exit(1);
    }
    res->width = width;
    res->height = height;
    res->type = type;
    if (res->type == MATRIX){
        res->u.matrix = (cell**) malloc(sizeof(cell*) * height);
        unsigned i, j;
        for (i = 0; i < height; i++){
            res->u.matrix[i] = (cell*) malloc(sizeof(cell) * width);
            for (j = 0; j < width; j++){
                res->u.matrix[i][j] = EMPTY;
            }
        }
        return res;
    } else {
        unsigned len = length(width, height);
        res->u.bits = (unsigned int*) malloc(sizeof(unsigned int) * len);
        unsigned i;
        for (i = 0; i < len; i++){
            res->u.bits[i] = 0;
        }
    }
    return res;
}

void board_free(board* b){
    unsigned i;
    if (b->type == BITS){
        free(b->u.bits);
        free(b);
    } else {
        for (i = 0; i < b->height; i++){
            free(b->u.matrix[i]);
        }
        free(b->u.matrix);
        free(b);
    }
}

/* print_labels: helper to print labels of matrix */
void print_labels(unsigned val){
    if (val < 10){
        printf("%u", val);
    } else if (val < 36){
        printf("%c", 'A' + val - 10);
    } else if (val < 62){
        printf("%c", 'a' + val - 36);
    } else {
        printf("?");
    }
}

void board_show(board* b){
    printf("  ");
    unsigned i, j, k;
    for (k = 0; k < b->width; k++){
        print_labels(k);
    }
    printf("\n\n");
    pos p;
    for (i = 0; i < b->height; i++){
        for (j = 0; j <= b->width; j++){
            if (j == 0){
                print_labels(i);
                printf(" ");
            } else {
                p = make_pos(i, j - 1);
                cell piece = board_get(b, p);
                if (piece == BLACK){
                    printf("*");
                } else if (piece == WHITE){
                    printf("o");
                } else if (piece == EMPTY){
                    printf(".");
                }
            }
            if (j == b->width){
                printf("\n");
            }
        }
    }
}

/* get_cell: gets the position of a requested point in bit representation */
unsigned get_cell(board* b, pos p){
    unsigned position = (2 * p.r * b->width) + (p.c * 2);
    if (position > b->width * b->height * 2){
        fprintf(stderr, "get_pos: invalid pos (out of range)\n");
        exit(1);
    }
    unsigned i, shift;
    i = position / 32;
    shift = position % 32;
    return (b->u.bits[i] >> shift) & 3;
}

cell board_get(board* b, pos p){
    if (b->type == BITS){
        unsigned val = get_cell(b, p);
        if (val == 0){
            return EMPTY;
        } else if (val == 1){
            return BLACK;
        } else if (val == 2){
            return WHITE;
        } else {
            fprintf(stderr, "board_get: invalid bit representation\n");
            exit(1);
        }
    } else {
        if (p.r < b->height && p.c < b->width){
            return b->u.matrix[p.r][p.c];
        } else {
            fprintf(stderr, "board_get: index out of range\n");
            exit(1);
        }
    }
}

void board_set(board* b, pos p, cell c){
    if (b->type == BITS){
        unsigned replacement;
        if (c == EMPTY){
            replacement = 0;
        } else if (c == BLACK){
            replacement = 1;
        } else {
            replacement = 2;
        }
        unsigned position = (2 * p.r * b->width) + (p.c * 2);
        if (position > b->width * b->height * 2){
            fprintf(stderr, "get_pos: invalid pos (out of range)\n");
            exit(1);
        }
        unsigned i, shift;
        i = position / 32;
        shift = position % 32;
        b->u.bits[i] = (b->u.bits[i] & ~(3 << shift)) |
                                (replacement << shift);
    } else {
        if (p.r < b->height && p.c < b->width){
            b->u.matrix[p.r][p.c] = c;
        } else {
            fprintf(stderr, "board_set: index out of range\n");
            exit(1);
        }
    }
}
