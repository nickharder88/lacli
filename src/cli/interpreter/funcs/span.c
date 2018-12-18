#include <stdlib.h>

#include "span.h"
#include "aug.h"
#include "rref.h"

Rval* span_handler(Rval** args, unsigned nargs) {
    char row_checked = 0;
    unsigned i, nmatrices, col_i, row_check;
    Matrix *arg, *v;
    Matrix **cols, **arr;
    Rval* val;

    if(nargs < 2 || args[0]->type != RMATRIX) {
        printf("Usage: span(v, matrices...)\n");
        return NULL;
    }

    nmatrices = 0;
    for(i = 1; i < nargs; i++) {
        if(args[i]->type == RMATRIX) {
            if(row_checked) {
                if(args[i]->value.matrix->nrows != row_check) {
                    printf("Error: matrices must have the same number of rows\n");
                    return NULL;
                }
            } else {
                row_check = args[i]->value.matrix->nrows;
                row_checked = 1;
            }
            nmatrices++;
        } else if(args[i]->type == RMATRIX_ARRAY) {
            arr = args[i]->value.array.matrix_array;
            for(col_i = 0; col_i < args[i]->value.array.length; col_i++) {
                if(row_checked) {
                    if(arr[col_i]->nrows != row_check) {
                        printf("Error: matrices must have the same number of rows\n");
                        return NULL;
                    }
                } else {
                    row_check = arr[col_i]->nrows;
                    row_checked = 1;
                }
            }
            nmatrices += args[i]->value.array.length;
        } else {
            printf("Usage: span(v, matrix...)\n");
            return NULL;
        }
    }

    v = args[0]->value.matrix;
    cols = malloc(nmatrices * sizeof(struct Matrix *));

    nmatrices = 0;
    for(i = 1; i < nargs; i++) {
        if(args[i]->type == RMATRIX) {
            cols[nmatrices++] = args[i]->value.matrix;
        } else {
            arr = args[i]->value.array.matrix_array;
            for(col_i = 0; col_i < args[i]->value.array.length; col_i++) {
                cols[nmatrices] = arr[nmatrices];
                nmatrices++;
            }
        }
    }

    val = span(v, cols, nmatrices);
    free(cols);
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
    v_rref = rref_aug(v_aug->value.matrix);
    sol = v_rref->value.matrix;

    for(row_i = 0; row_i < sol->nrows; row_i++) {
        row = sol->values.rows[row_i];
        nvals = 0;
        for(col_i = 0; col_i < sol->ncols - 1; col_i++) {
            if(row->values.literals[col_i] != 0)
                nvals++;
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
    return rval_make_boolean(TRUE);
}
