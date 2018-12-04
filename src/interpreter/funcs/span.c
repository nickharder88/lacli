#include <stdlib.h>

#include "span.h"
#include "aug.h"
#include "rref.h"

Rval* span_handler(Rval** args, unsigned nargs) {
    unsigned i;
    Matrix **col_vectors, *v;
    Rval* val;

    if(nargs < 2 || args[0]->type != RMATRIX) {
        printf("Usage: span(vector, vector...)\n");
        return NULL;
    }

    v = args[i]->value.matrix;
    col_vectors = malloc(nargs * sizeof(struct Matrix *));

    for(i = 1; i < nargs; i++) {
        if(args[i]->type != RMATRIX) {
            printf("Usage: span(matrix, ...)\n");
            free(col_vectors);
            return NULL;
        }

        col_vectors[i] = args[i]->value.matrix;
    }

    val = span(v, col_vectors, nargs-1);
    free(col_vectors);
    return val;
}

Rval* span(Matrix* v, Matrix** columns, unsigned ncols) {
    unsigned row_i, col_i, nvals;
    Rval *m_aug, *v_aug, *v_rref;
    Matrix *m, *marr[2], *sol, *row;

    m_aug = aug(columns, ncols);

    marr[0] = m_aug->value.matrix;
    marr[1] = v;

    v_aug = aug(marr, 2);
    v_rref = rref(v_aug->value.matrix);
    sol = v_rref->value.matrix;

    for(row_i = 0; row_i < sol->nrows; row_i++) {
        row = sol->values.rows[row_i];
        nvals = 0;
        for(col_i = 0; col_i < sol->ncols - 1; col_i++) {
            if(row->values.literals[col_i] != 0)
                nvals++;

            /* TODO is this necessary?
            if(nvals > 1) {
                rval_destroy(m_aug);
                rval_destroy(v_aug);
                rval_destroy(v_rref);
                return rval_make_boolean(TRUE);
            }
            */
        }

        /* inconsistent system of linear equations */
        if(nvals == 0 && row->values.literals[col_i] != 0) {
            rval_destroy(m_aug);
            rval_destroy(v_aug);
            rval_destroy(v_rref);
            return rval_make_boolean(FALSE);
        }
    }

    rval_destroy(m_aug);
    rval_destroy(v_aug);
    rval_destroy(v_rref);
    return rval_make_boolean(FALSE);
}
