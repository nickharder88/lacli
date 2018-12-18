#include <stdlib.h>

#include "dim.h"
#include "aug.h"
#include "rref.h"
#include "rank.h"

Rval* dim_handler(Rval** args, unsigned nargs) {
    unsigned i, ncols, col_i;
    Matrix* arg;
    Matrix **cols, **arr;

    if(nargs < 2) {
        printf("Usage: dim(matrix)\n");
        return NULL;
    }

    ncols = 0;
    for(i = 0; i < nargs; i++) {
        if(args[i]->type == RMATRIX) {
            ncols++;
        } else if(args[i]->type == RMATRIX_ARRAY) {
            ncols += args[i]->value.array.length;
        } else {
            printf("Usage: dim(col_matrix, col_matrix...)\n");
            return NULL;
        }
    }

    cols = malloc(ncols * sizeof(struct Matrix *));

    ncols = 0;
    for(i = 0; i < nargs; i++) {
        if(args[i]->type == RMATRIX) {
            cols[ncols++] = args[i]->value.matrix;
        } else {
            arr = args[i]->value.array.matrix_array;
            for(col_i = 0; col_i < args[i]->value.array.length; col_i++) {
                cols[ncols] = arr[ncols];
                ncols++;
            }
        }
    }

    return dim(cols, ncols);
}

/* 
 * dim(dimension) is the smallest number of vectors that span M 
 */
Rval* dim(Matrix** cols, unsigned ncols) {
    Rval *col_aug, *col_rref, *col_dim;
    Matrix *m;

    col_aug = aug(cols, ncols);
    col_rref = rref_aug(col_aug->value.matrix);
    col_dim = rank(col_rref->value.matrix);
    return rval_make_literal(col_dim->value.literal);
}
