#include <stdlib.h>

#include "null.h"
#include "rref.h"
#include "rank.h"

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
    unsigned mrank, len, i, row_i, col_i, pivot, vec_i, j;
    double value;
    Matrix** nspace, *vec, *row;
    Rval* val;

    /*
     * Let A be an mxn matrix with rank l.
     * The null space of A is the span of n-l vectors
     */

    val = rref(m);
    mrank = rank_rref(val->value.matrix);
    len = m->ncols - mrank;

    nspace = malloc(len * sizeof(struct Matrix *));
    for(i = 0; i < len; i++) {
        nspace[i] = vec = matrix_create_dim(m->ncols, 1);

        for(j = 0; j < m->ncols; j++) {
            vec->values.rows[j]->values.literals[0] = 0;
        }

        vec->values.rows[mrank+i]->values.literals[0] = 1;
    }

    for(row_i = 0; row_i < mrank; row_i++) {
        row = m->values.rows[row_i];
        pivot = get_pivot(row);

        for(i = 0; i < len; i++) {
            nspace[i]->values.rows[pivot]->values.literals[0] = -1 * row->values.literals[mrank + row_i];
        }

        for(col_i = pivot+1; col_i < m->ncols; col_i++) {
            nspace[i]->values.literals[pivot] = -1 * row->values.literals[col_i];
        }
    }
    return rval_make_matrix_array(nspace, len);
}
