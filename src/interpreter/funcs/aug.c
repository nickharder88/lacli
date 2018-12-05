#include <ctype.h>
#include <stdlib.h>

#include "aug.h"

Rval* aug_handler(Rval** args, unsigned nargs) {
    unsigned i;
    Matrix** marr;

    if(nargs < 2) {
        printf("Usage: aug(matrix, matrix...)\n");
        return NULL;
    }

    marr = malloc(nargs * sizeof(struct Matrix *));

    for(i = 0; i < nargs; i++) {
        if(args[i]->type != RMATRIX) {
            printf("Usage: aug(matrix, matrix...)\n");
            free(marr);
            return NULL;
        }

        marr[i] = args[i]->value.matrix;
    }

    return aug(marr, nargs);
}

Rval* aug(Matrix** marr, unsigned nmatrices) {
    Matrix *m, *row, *m_row, *copy_row, *m_cur;
    unsigned m_i, row_i, col_i, i, m_col_i, nrows, ncols = 0;

    if(nmatrices == 1)
        return rval_make_matrix(matrix_copy(marr[0]));

    nrows = marr[0]->nrows;

    for(i = 0; i < nmatrices; i++) {
        if(marr[i]->nrows != nrows) {
            printf("Error: matrices must have the same number of rows.\n");
            return NULL;
        }

        ncols += marr[i]->ncols;
    }

    m = matrix_create_dim(nrows, ncols);

    if(m->nrows == 1) {
        m_col_i = 0;
        for(m_i = 0; m_i < nmatrices; m_i++) {
            m_cur = marr[m_i];
            for(col_i = 0; col_i < m_cur->ncols; col_i++)
                m->values.literals[m_col_i++] = m_cur->values.literals[col_i];
        }
        return rval_make_matrix(m);
    }

    for(row_i = 0; row_i < nrows; row_i++) {
        m_col_i = 0;
        for(m_i = 0; m_i < nmatrices; m_i++) {
            m_cur = marr[m_i];

            row = m_cur->values.rows[row_i];
            m_row = m->values.rows[row_i];
            for(col_i = 0; col_i < m_cur->ncols; col_i++)
                m_row->values.literals[m_col_i++] = row->values.literals[col_i];
        }
    }

    return rval_make_matrix(m);
}
