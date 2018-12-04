#include <stdlib.h>

#include "rank.h"
#include "rref.h"

Rval* rank_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: rank(matrix)\n");
        return NULL;
    }

    return rank(args[0]->value.matrix);
}

unsigned rank_rref(Matrix *m) {
    Matrix* row;
    unsigned mrank = 0, row_i, col_i, pivot;

    for(row_i = 0; row_i < m->nrows; row_i++) {
        row = m->values.rows[row_i];
        pivot = get_pivot(row);
        for(col_i = pivot; col_i < row->ncols; col_i++)
            if(row->values.literals[col_i] != 0) {
                mrank++;
                break;
            }
    }
    return mrank;
}

Rval* rank(Matrix* m) {
    unsigned mrank;
    Rval* m_rref = rref(m);
    m = m_rref->value.matrix;

    mrank = rank_rref(m);

    rval_destroy(m_rref);
    return rval_make_literal(mrank);
}
