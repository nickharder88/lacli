#include "trace.h"

Rval* trace_handler(Rval* args, unsigned nargs) {
    if(nargs != 1 || args[0].type != RMATRIX) {
        printf("Usage: trace(matrix)\n");
        return NULL;
    }

    return trace(args[0].value.matrix);
}

Rval* trace(Matrix* m) {
    unsigned i;
    double val = 0;

    // TODO 1x1 matrix
    for(i = 0; i < m->nrows; i++)
        val += m->values.rows[i].values.literals[i];
    return rval_make_literal(val);
}
