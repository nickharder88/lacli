#include <ctype.h>
#include <stdlib.h>

#include "I.h"

Rval* I_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RLITERAL) {
        printf("Usage: I(rows)");
        return NULL;
    }

    return I(args[0]->value.literal);
}

Rval* I(unsigned nrows) {
    Matrix *m, *row;
    unsigned row_i, col_i;

    m = matrix_create_dim(nrows, nrows);
    if(nrows == 1) {
        m->values.literals[0] = 1;
    } else {
        for(row_i = 0; row_i < nrows; row_i++) {
            row = m->values.rows[row_i];
            for(col_i = 0; col_i < nrows; col_i++)
                row->values.literals[col_i] = 0;
            row->values.literals[row_i] = 1;
        }
    }
    
    return rval_make_matrix(m);
}
