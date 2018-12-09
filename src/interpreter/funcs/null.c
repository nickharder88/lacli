#include <stdlib.h>

#include "null.h"
#include "rref.h"
#include "rank.h"
#include "../../util.h"

/*
 * null space is the set of solutions to the homogeneous
 * system of linear equations Ax=0
 */

Rval* null_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: null(matrix)\n");
        return NULL;
    }

    return null(args[0]->value.matrix);
}

Rval* null(Matrix* m) {
    char is_pivot;
    unsigned mrank, len, i, j, row_i, col_i, pivot, vec_i;
    unsigned *pivots, *nonpivots;
    double value;
    Matrix** nspace, *vec, *row;
    Rval* val;

    /*
     * Let A be an mxn matrix with rank l.
     * The null space of A is the span of n-l vectors
     */

    val = rref(m);
    m = val->value.matrix;
    mrank = rank_rref(val->value.matrix);
    len = m->ncols - mrank;

    nonpivots = malloc(len * sizeof(unsigned));
    pivots = malloc(mrank * sizeof(unsigned));
    j = 0;
    col_i = 0;
    for(i = 0; i < mrank; i++, j++) {
        row = m->values.rows[i];
        pivot = get_pivot(row);
        pivots[i] = pivot;
        if(j == pivot) {
            continue;
        }

        while(j < pivot)
            nonpivots[col_i++] = j++;
    }
    for(; col_i < len; col_i++)
        nonpivots[col_i] = j++;

    /* make 0 and 1s in matrices */
    nspace = malloc(len * sizeof(struct Matrix *));
    for(i = 0; i < len; i++) {
        nspace[i] = vec = matrix_create_dim(m->ncols, 1);

        for(row_i = 0; row_i < m->ncols; row_i++)
            vec->values.rows[row_i]->values.literals[0] = 0;
        vec->values.rows[nonpivots[i]]->values.literals[0] = 1;
    }

    for(row_i = 0; row_i < mrank; row_i++) {
        row = m->values.rows[row_i];
        pivot = pivots[row_i];

        /*
         * e = [[1,0,-2,-1], [0,1,8,2]]
         * x1 = 2x3 + x4
         * x2 = -8x3 -2x4
         *
         * [[2],[-8],[1],[0]]
         * [[1],[-2],[0],[1]]
         */

        for(i = 0; i < len; i++)
            nspace[i]->values.rows[pivot]->values.literals[0] = round_if_insignificant(-1 * row->values.literals[nonpivots[i]]);
    }

    free(nonpivots);
    free(pivots);

    if(len == 1)
        return rval_make_matrix(nspace[0]);
    return rval_make_matrix_array(nspace, len);
}
