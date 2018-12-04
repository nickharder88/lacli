#include <stdlib.h>

#include "span.h"
#include "aug.h"
#include "rref.h"

Rval* span_handler(Rval** args, unsigned nargs) {
    unsigned i, m_i, length, ncols;
    Matrix **col_vectors, *v, **marr;
    Rval *val, *arg;

    if(nargs < 2 || args[0]->type != RMATRIX) {
        printf("Usage: span(vector, vector...)\n");
        return NULL;
    }

    ncols = 0;
    for(i = 1; i < nargs; i++) {
        arg = args[i];
        if(arg->type == RMATRIX) {
            ncols++;
        } else if(arg->type == RMATRIX_ARRAY) {
            ncols += arg->value.array.length;
        } else {
            printf("Usage: span(matrix, ...)\n");
            return NULL;
        }
    }

    v = args[0]->value.matrix;
    col_vectors = malloc(ncols * sizeof(struct Matrix *));

    for(i = 1; i < nargs; i++) {
        arg = args[i];
        if(arg->type == RMATRIX) {
            col_vectors[i] = args[i]->value.matrix;
        } else if(arg->type == RMATRIX_ARRAY) {
            length = arg->value.array.length;
            marr = arg->value.array.matrix_array;
            for(m_i = 0; m_i < length; m_i++)
                col_vectors[m_i] = marr[m_i];
        } else {
            printf("Usage: span(matrix, ...)\n");
            free(col_vectors);
            return NULL;
        }
    }

    val = span(v, col_vectors, ncols);
    free(col_vectors);
    return val;
}

Rval* span(Matrix* v, Matrix** columns, unsigned ncols) {
    unsigned row_i, col_i, nvals;
    Rval *m_aug, *v_aug, *v_rref;
    Matrix *m, *marr[2], *sol, *row, *mmult;

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
