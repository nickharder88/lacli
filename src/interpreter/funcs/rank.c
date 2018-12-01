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

Rval* rank(Matrix* m) {
    unsigned mrank = 0, i;
    unsigned pivot;
    Matrix* row;
    Rval* m_rref = rref(m);
    m = m_rref->value.matrix;

    for(i = 0; i < m->nrows; i++) {
        row = m->values.rows[i];
        pivot = get_pivot(row);
        if(row->values.literals[i] < m->ncols)
            mrank++;
    }
    
    rval_destroy(m_rref);
    return rval_make_literal(mrank);
}
