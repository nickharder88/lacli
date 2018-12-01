#include <ctype.h>
#include <stdlib.h>

#include "aug.h"

Rval* aug_handler(Rval** args, unsigned nargs) {
    if(nargs != 2) {
        printf("Usage: aug(matrix, matrix)");
        return NULL;
    }

    if(args[0]->type != RMATRIX || args[1]->type != RMATRIX) {
        printf("Usage: aug(matrix, matrix)");
        return NULL;
    }

    return aug(args[0]->value.matrix, args[1]->value.matrix);
}

Rval* aug(Matrix* m1, Matrix* m2) {
    Matrix *m, *row, *copy_row;
    unsigned row_i, col_i, ncols;

    if(m1->nrows != m2->nrows) {
        printf("Error: matrices must have the same number of rows.\n");
        return NULL;
    }

    ncols = m1->ncols + m2->ncols;
    m = matrix_create_dim(m1->nrows, ncols);

    if(m->nrows == 1) {
        for(col_i = 0; col_i < m1->ncols; col_i++) {
            m->values.literals[col_i] = m1->values.literals[col_i];
        }
        for(col_i = 0; col_i < m2->ncols; col_i++) {
            m->values.literals[col_i + m1->ncols] = m2->values.literals[col_i];
        }
    } else {
        for(row_i = 0; row_i < m1->nrows; row_i++) {
            row = m->values.rows[row_i];
            copy_row = m1->values.rows[row_i];
            for(col_i = 0; col_i < m1->ncols; row_i++) {
                row->values.literals[col_i] = copy_row->values.literals[col_i];
            }
        }

        for(row_i = 0; row_i < m2->nrows; row_i++) {
            row = m->values.rows[row_i];
            copy_row = m1->values.rows[row_i];
            for(col_i = 0; col_i < m1->ncols; row_i++) {
                row->values.literals[col_i + m1->ncols] = copy_row->values.literals[col_i];
            }
        }
    }

    return rval_make_matrix(m);
}
