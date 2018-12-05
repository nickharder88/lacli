#include <stdlib.h>

#include "basis.h"
#include "span.h"
#include "linind.h"
#include "aug.h"
#include "null.h"

Rval* basis_handler(Rval** args, unsigned nargs) {
    char row_checked;
    unsigned i, nmatrices, col_i, row_check;
    Matrix* arg;
    Matrix **cols, **arr;

    if(nargs < 1) {
        printf("Usage: basis(matrix...)\n");
        return NULL;
    }

    nmatrices = 0;
    for(i = 0; i < nargs; i++) {
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
            printf("Usage: basis(col_matrix, col_matrix...)\n");
            return NULL;
        }
    }

    cols = malloc(nmatrices * sizeof(struct Matrix *));

    nmatrices = 0;
    for(i = 0; i < nargs; i++) {
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

    return basis(cols, nmatrices);
}

Rval* is_basis_handler(Rval** args, unsigned nargs) {
    unsigned i, ncols, col_i;
    Matrix *arg, *space;
    Matrix **cols, **arr;

    if(nargs < 2 || args[0]->type != RMATRIX) {
        printf("Usage: is_basis(vspace, col_matrix...)\n");
        return NULL;
    }

    ncols = 0;
    for(i = 1; i < nargs; i++) {
        if(args[i]->type == RMATRIX) {
            ncols++;
        } else if(args[i]->type == RMATRIX_ARRAY) {
            ncols += args[i]->value.array.length;
        } else {
            printf("Usage: is_basis(vspace, col_matrix...)\n");
            return NULL;
        }
    }

    space = args[0]->value.matrix;
    cols = malloc(ncols * sizeof(struct Matrix *));

    ncols = 0;
    for(i = 1; i < nargs; i++) {
        if(args[i]->type == RMATRIX) {
            cols[ncols++] = args[i]->value.matrix;
        } else {
            arr = args[i]->value.array.matrix_array;
            for(col_i = 0; col_i < args[i]->value.array.length; col_i++) {
                cols[ncols] = arr[ncols];
                ncols++;
            }
        }
    }

    return is_basis(space, cols, ncols);
}

Rval* basis(Matrix** cols, unsigned ncols) {
    unsigned i, j, col_rank_val, nrows, pivot;
    Rval *col_aug, *col_rref, *col_rank;
    Matrix **basis_cols, *row, *m_rref, *vec;

    col_aug = aug(cols, ncols);

/* Possible copy of null(matrix)
    col_rref = rref(col_aug->value.matrix);
    m_rref = col_rref->value.matrix;

    col_rank = rank(col_rref->value.matrix);

    col_rank_val = col_rank->value.literal;
    nrows = col_rref->value.matrix->nrows;

    basis_cols = malloc(nrows * sizeof(struct Matrix *));
    for(i = 0; i < nrows; i++) {
        basis_cols[i] = vec = matrix_create_dim(ncols, 1);

        for(j = 0; j < vec->nrows; j++) {
            vec->values.rows[j]->values.literals[0] = 0;
        }

        vec->values.rows[col_rank_val+i]->values.literals[0] = 1;
    }

    for(i = 0; i < col_rank_val; i++) {
        row = m_rref->values.rows[i];
        pivot = get_pivot(row);

        for(j = 0; j < nrows; j++)
            basis_cols[j]->values.rows[pivot]->values.literals[0] = -1 * row->values.literals[col_rank_val+j];
    }

*/
    return null(col_aug->value.matrix);
}

/*
 * a set of vectors in a vector space W is a basis for W iff the set B
 * is linearly independent and spans W
 */
Rval* is_basis(Matrix* space, Matrix** cols, unsigned ncols) {
    Rval *is_span, *is_linind;

    is_span = span(space, cols, ncols);
    if(is_span->value.boolean == FALSE)
        return is_span;

    is_linind = linind(cols, ncols);
    if(is_linind->value.boolean == FALSE)
        return is_linind;

    is_linind->value.boolean = TRUE;
    return is_linind;
}
