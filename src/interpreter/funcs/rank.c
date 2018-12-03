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
    unsigned mrank = 0, i, pivot;

    for(i = 0; i < m->nrows; i++) {
        row = m->values.rows[i];
        pivot = get_pivot(row);
        if(row->values.literals[i] < m->ncols)
            mrank++;
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
