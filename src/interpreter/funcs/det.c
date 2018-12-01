#include "det.h"

Rval* det_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: det(matrix)\n");
        return NULL;
    }

    return det(args[0]->value.matrix);
}

Rval* det(Matrix* m) {
    double val;
    if(m->nrows != 2 || m->ncols != 2) {
        printf("Usage: can only get determinant of 2x2 matrices.\n");
        return NULL;
    }

    val = (m->values.rows[0]->values.literals[0] * m->values.rows[1]->values.literals[1])
        - (m->values.rows[0]->values.literals[1] * m->values.rows[0]->values.literals[1]);

    return rval_make_literal(val);
}
