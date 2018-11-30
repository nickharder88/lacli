#include "transpose.h"

Rval* transpose_handler(Rval* args, unsigned nargs) {
    if(nargs != 1 || args[0].type != RMATRIX) {
        printf("Usage: transpose(matrix)\n");
        return NULL;
    }

    return transpose(args[0].value.matrix);
}

Rval* transpose(Matrix* m) {
    Matrix *t, *row;
    unsigned row_i, col_i;
    unsigned ncols = m->ncols;

    if(m->ncols != m->nrows) {
        printf("Usage: matrix must be square.\n");
        return NULL;
    }

    t = matrix_create_empty(m->ncols, m->nrows);

    // TODO 1 row matrix
    for(row_i = 0; row_i < t->nrows; row_i++) {
        for(col_i = 0; col_i < ncols; col_i++)
            t->values.rows[row_i].values.literals[col_i] = m->values.rows[col_i].values.literals[row_i];
    }

    return rval_make_matrix(t);
}
