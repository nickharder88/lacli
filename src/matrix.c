#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "matrix.h"
#include "util.h"

Matrix* matrix_create(void) {
    Matrix* matrix;

    matrix = (Matrix*)malloc(sizeof(Matrix));

    matrix->values.literals = NULL;
    matrix->nrows = 0;
    matrix->ncols = 0;

    return matrix;
}

Matrix* matrix_create_dim_uninitialized(unsigned nrows) {
    unsigned row_i;
    Matrix *row, *m = malloc(sizeof(Matrix));
    m->nrows = nrows;
    m->ncols = 1;
    if(nrows > 1) {
        m->values.rows = malloc(nrows * sizeof(struct Matrix *));
        for(row_i = 0; row_i < nrows; row_i++)
            m->values.rows[row_i] = NULL;
    }
    return m;
}

Matrix* matrix_create_dim(unsigned nrows, unsigned ncols) {
    unsigned row_i;
    Matrix *row, *m = malloc(sizeof(Matrix));
    m->nrows = nrows;
    m->ncols = ncols;
    if(nrows == 1) {
        m->values.literals = malloc(ncols * sizeof(double));
    } else {
        m->values.rows = malloc(nrows * sizeof(struct Matrix *));

        for(row_i = 0; row_i < nrows; row_i++) {
            row = m->values.rows[row_i] = malloc(sizeof(struct Matrix));
            row->nrows = 1;
            row->ncols = ncols;
            row->values.literals = malloc(ncols * sizeof(double));
        }
    }
    return m;
}

Matrix* matrix_create_zero(unsigned nrows, unsigned ncols) {
    unsigned row_i, col_i;
    Matrix* row;
    Matrix* m = matrix_create_dim(nrows, ncols);

    if(nrows == 1) {
        for(col_i = 0; col_i < ncols; col_i++) {
            m->values.literals[col_i] = 0;
        }
    } else {
        for(row_i = 0; row_i < m->nrows; row_i++) {
            row = m->values.rows[row_i];
            for(col_i = 0; col_i < m->ncols; col_i++)
                row->values.literals[col_i] = 0;
        }
    }
    
    return m;
}

void matrix_destroy(void* data) {
    unsigned i;
    Matrix *row, *m = (Matrix*)data;

    if(m->nrows == 1) {
        if(m->values.literals != NULL)
            free(m->values.literals);
        free(m);
    } else {
        if(m->values.rows != NULL) {
            for(i = 0; i < m->nrows; i++) {
                row = m->values.rows[i];
                if(row != NULL) {
                    if(row->values.literals != NULL) {
                        free(row->values.literals);
                    }
                    free(row);
                }
             }
             free(m->values.rows);
        }
        free(m);
    }
}

Matrix* matrix_add(Matrix* a, Matrix* b) {
    Matrix *m, *row;
    unsigned row_i, col_i;

    if(a->ncols != b->ncols || a->nrows != b->nrows) {
        printf("Error: matrices have different dimensions\n");
        return NULL;
    }

    m = matrix_create_dim(a->nrows, a->ncols);
    if(m->nrows == 1) {
        for(col_i = 0; col_i < m->ncols; col_i++)
            m->values.literals[col_i] = a->values.literals[col_i] + b->values.literals[col_i];
    } else {
        for(row_i = 0; row_i < a->nrows; row_i++) {
            row = m->values.rows[row_i];
            for(col_i = 0; col_i < a->ncols; col_i++) {
                row->values.literals[col_i] = a->values.rows[row_i]->values.literals[col_i]
                                 + b->values.rows[row_i]->values.literals[col_i];
            }
        }
    }
    
    return m;
}

Matrix* matrix_subtract(Matrix* a, Matrix* b) {
    Matrix *m, *row;
    unsigned row_i, col_i;

    if(a->ncols != b->ncols || a->nrows != b->nrows) {
        printf("Error: matrices have different dimensions\n");
        return NULL;
    }

    m = matrix_create_dim(a->nrows, a->ncols);
    if(m->nrows == 1) {
        for(col_i = 0; col_i < m->ncols; col_i++) {
            m->values.literals[col_i] = a->values.literals[col_i] - b->values.literals[col_i];
        }
    } else {
        for(row_i = 0; row_i < a->nrows; row_i++) {
            row = m->values.rows[row_i];
            for(col_i = 0; col_i < a->ncols; col_i++)
                row->values.literals[col_i] = a->values.rows[row_i]->values.literals[col_i] - b->values.rows[row_i]->values.literals[col_i];
        }

    }

    return m;
}


Matrix* matrix_multiply(Matrix* a, Matrix* b) {
    Matrix *m;
    double *lita, *litb, *litm;
    unsigned row_i, col_i, col_j;
    double sum;

    if(a->ncols != b->nrows) {
        printf("Error: matrices cannot be multiplied\n");
        return NULL;
    }

    m = matrix_create_dim(a->nrows, b->ncols);

    for(row_i = 0; row_i < a->nrows; row_i++) {
        /* row we are calculating values for */
        litm = m->values.rows[row_i]->values.literals;

        /* row from a we are multiplying with b columns */
        lita = a->values.rows[row_i]->values.literals;

        /* col_i holds position in litm to place sum */
        for(col_i = 0; col_i < m->ncols; col_i++) {

            sum = 0;
            for(col_j = 0; col_j < a->ncols; col_j++) {
                /* row from b we are multiplying by a rows */
                litb = b->values.rows[col_j]->values.literals;

                sum += lita[col_j] * litb[col_i];
            }

            litm[col_i] = round_if_insignificant(sum);
        }
    }

    return m;
}

void matrix_print(Matrix* m) {
    unsigned row_i, col_i;
    Matrix* row;

    if(m == NULL) {
        printf("Error: cannot print matrix. Matrix does not exist.\n");
        return;
    }

    if(m->nrows == 1) {
        // 1 Dimensional
        putchar('\t');
        for(col_i = 0; col_i < m->ncols - 1; col_i++)
            printf("%g\t", m->values.literals[col_i]);
        printf("%g\n", m->values.literals[col_i]);
    } else {
        for(row_i = 0; row_i < m->nrows; row_i++) {
            row = m->values.rows[row_i];
            putchar('\t');
            for(col_i = 0; col_i < row->ncols - 1; col_i++)
                printf("%g\t", row->values.literals[col_i]);
            printf("%g\n", row->values.literals[col_i]);
        }
    }
}

unsigned matrix_max_row(Matrix** m, unsigned length) {
    unsigned i, max = 0;

    for(i = 0; i < length; i++)
        if(m[i]->nrows > max)
            max = m[i]->nrows;
    
    return max;
}

void matrix_print_multiple(Matrix** m, unsigned length) {
    unsigned i;
    /*
     * 1 | 0
     * 0 | 1
     */
    for(i = 0; i < length; i++) {
        matrix_print(m[i]);
        printf("\n");
    }
}

/* deep copies the matrix. if identifier is null, it uses the name
 * of the original matrix.
 */
Matrix* matrix_copy(Matrix* m) {
    Matrix *copy, *rcopy;
    unsigned row_i, col_i;

    copy = matrix_create_dim(m->nrows, m->ncols);

    if(m->nrows == 1)
        for(col_i = 0; col_i < m->ncols; col_i++)
            copy->values.literals[col_i] = m->values.literals[col_i];
    else
        for(row_i = 0; row_i < m->nrows; row_i++) {
            rcopy = copy->values.rows[row_i];
            for(col_i = 0; col_i < m->ncols; col_i++)
                copy->values.rows[row_i]->values.literals[col_i] = m->values.rows[row_i]->values.literals[col_i];

        }
    return copy;
}

void matrix_slice_before(Matrix *m, unsigned col) {
    unsigned row_i, col_i;
    Matrix* row;
    double* vals;

    if(m->nrows == 1) {
        if((vals = realloc(m->values.literals, col * sizeof(double))) == NULL) {
            printf("Error: could not allocate memory. Try again.\n");
            return;
        }
    } else {
        for(row_i = 0; row_i < m->nrows; row_i++) {
            row = m->values.rows[row_i];
            for(col_i = 0; col_i < m->ncols; col_i++) {
                if((vals = realloc(row->values.literals, col * sizeof(double))) == NULL) {
                    printf("Error: could not allocate memory. Try again.\n");
                    return;
                }
            }
            row->ncols = col;
        }
    }
    m->ncols = col;
}

void matrix_slice_after(Matrix *m, unsigned col) {
    unsigned row_i, col_i, ncols = m->ncols - col;

    Matrix *row;
    double* vals;

    if(m->nrows == 1) {
        vals = malloc(ncols * sizeof(double));
        for(col_i = 0; col_i < col; col_i++) {
            vals[col_i] = m->values.literals[col + col_i];
        }
        free(m->values.literals);
        m->values.literals = vals;
    } else {
        for(row_i = 0; row_i < m->nrows; row_i++) {
            row = m->values.rows[row_i];
            vals = malloc(ncols * sizeof(double));
            for(col_i = 0; col_i < ncols; col_i++)
                vals[col_i] = row->values.literals[col + col_i];
            free(row->values.literals);
            row->values.literals = vals;
            row->ncols = ncols;
        }
    }

    m->ncols = ncols;
}

Matrix* matrix_multiply_constant(Matrix* m, double val) {
    unsigned col_i, row_i;
    Matrix* row;
    Matrix* copy = matrix_copy(m);

    if(m->nrows == 1) {
        for(col_i = 0; col_i < copy->ncols; col_i++)
            m->values.literals[col_i] *= val;
    } else {
        for(row_i = 0; row_i < copy->nrows; row_i++) {
            row = copy->values.rows[row_i];
            for(col_i = 0; col_i < copy->ncols; col_i++)
                row->values.literals[col_i] *= val;
        }
    }
    
    return copy;
}

char matrix_cmp(Matrix* m1, Matrix* m2) {
    Matrix *row1, *row2;
    unsigned row_i, col_i;

    if(m1->ncols != m2->ncols || m1->nrows != m2->nrows) {
        return -1;
    }

    if(m1->nrows == 1) {
        for(col_i = 0; col_i < m1->ncols; col_i++)
            if(double_cmp(m1->values.literals[col_i], row2->values.literals[col_i]) != 0)
                return -1;
    } else {
        for(row_i = 0; row_i < m1->nrows; row_i++) {
            row1 = m1->values.rows[row_i];
            row2 = m2->values.rows[row_i];
            for(col_i = 0; col_i < m1->ncols; col_i++)
                if(double_cmp(row1->values.literals[col_i], row2->values.literals[col_i]) != 0)
                    return -1;
        }
    }

    return 0;
}

Matrix* matrix_create_initializer_1D(double* vals, unsigned ncols) {
    unsigned col_i;
    Matrix* m;

    m = matrix_create_dim(1, ncols);
    for(col_i = 0; col_i < ncols; col_i++) {
        m->values.literals[col_i] = vals[col_i];
    }

    return m;
}

Matrix* matrix_create_initializer_2D(double** vals, unsigned nrows, unsigned ncols) {
    unsigned row_i, col_i;
    Matrix *m, *mrow;
    double* drow;

    m = matrix_create_dim(nrows, ncols);
    for(row_i = 0; row_i < nrows; row_i++) {
        mrow = m->values.rows[row_i];
        drow = vals[row_i];
        for(col_i = 0; col_i < ncols; col_i++)
            mrow->values.literals[col_i] = drow[col_i];
    }

    return m;
}

Matrix* matrix_copy_remove_row_col(Matrix* m, unsigned row, unsigned col) {
    /* returns copy of matrix with row and col removed */
    /* row_i/col_i hold index of m, copy_row_i,copy_col_i hold index of copy */
    Matrix *copy, *rcopy, *mrow;
    unsigned row_i, col_i, copy_row_i, copy_col_i;

    if(m->nrows == 1 || m->ncols == 1)
        return NULL;

    copy = matrix_create_dim(m->nrows-1, m->ncols-1);

    if(m->nrows == 2) {
        for(copy_col_i = col_i = 0; col_i < m->ncols; col_i++) {
            if(col_i == col)
                continue;
            copy->values.literals[copy_col_i] = m->values.rows[1-row]->values.literals[col_i];
            copy_col_i++;
        }

    } else {
        for(copy_row_i = row_i = 0; row_i < m->nrows; row_i++) {
            if(row_i == row)
                continue;
            mrow = m->values.rows[row_i];
            rcopy = copy->values.rows[copy_row_i];
            for(copy_col_i = col_i = 0; col_i < m->ncols; col_i++) {
                if(col_i == col)
                    continue;
                rcopy->values.literals[copy_col_i] = mrow->values.literals[col_i];
                copy_col_i++;
            }

            copy_row_i++;
        }
    }
    return copy;
}

Matrix* matrix_slice_below(Matrix* m, unsigned row) {
    unsigned i, row_i;
    Matrix** rows;
    Matrix* copy = matrix_create_dim_uninitialized(row);
    copy->ncols = m->ncols;

    for(row_i = 0; row_i < row; row_i++)
        copy->values.rows[row_i] = matrix_copy(m->values.rows[row_i]);

    return copy;
}
