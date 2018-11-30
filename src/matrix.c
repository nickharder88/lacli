#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "matrix.h"

/*
 * Returns rows by cols Matrix without a name
 */
Matrix* matrix_create() {
    Matrix* matrix;

    matrix = (Matrix*)malloc(sizeof(Matrix));

    matrix->dim = 0;
    matrix->values.literals = NULL;
    matrix->nrows = 0;
    matrix->ncols = 0;

    return matrix;
}

Matrix* matrix_create_dim(unsigned nrows, unsigned ncols) {
    unsigned row_i;
    Matrix *row, *m = malloc(sizeof(Matrix));
    m->nrows = nrows;
    m->ncols = ncols;
    if(nrows == 1) {
        m->dim = 1;
        m->values.literals = malloc(ncols * sizeof(double));
    } else {
        m->dim = 2;
        m->values.rows = malloc(nrows * sizeof(struct Matrix));

        for(row_i = 0; row_i < nrows; row_i++) {
            row = m->values.rows + row_i;
            row->nrows = 1;
            row->ncols = ncols;
            row->values.literals = malloc(sizeof(double));
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
            row = m->values.rows + row_i;
            for(col_i = 0; col_i < m->ncols; col_i++)
                row->values.literals[col_i] = 0;
        }
    }
    
    return m;
}

void matrix_destroy(void* data) {
    unsigned i;
    Matrix *row, *m = (Matrix*)data;

    switch(m->dim) {
        case 1:
            if(m->values.literals != NULL)
                free(m->values.literals);
            free(m);
            break;
        case 2:
            if(m->values.rows != NULL) {
                for(i = 0; i < m->nrows; i++) {
                    row = m->values.rows + i;
                    if(row->values.literals != NULL) {
                        free(row->values.literals);
                    }
                }
                free(m->values.rows);
            }
            free(m);
            break;
        default:
            //ERR
            break;
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
            row = m->values.rows + row_i;
            for(col_i = 0; col_i < a->ncols; col_i++) {
                row->values.literals[col_i] = a->values.rows[row_i].values.literals[col_i]
                                 + b->values.rows[row_i].values.literals[col_i];
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
            row = m->values.rows + row_i;
            for(col_i = 0; col_i < a->ncols; col_i++)
                row->values.literals[col_i] = a->values.rows[row_i].values.literals[col_i] - b->values.rows[row_i].values.literals[col_i];
        }

    }

    return a;
}


Matrix* matrix_multiply(Matrix* a, Matrix* b) {
    Matrix *m, *row;
    unsigned row_i, col_i, col_j;
    int sum;

    if(a->ncols != b->nrows) {
        printf("Error: matrices cannot be multiplied\n");
        return NULL;
    }

    m = matrix_create_dim(a->nrows, b->ncols);

    for(row_i = 0; row_i < a->nrows; row_i++) {
        row = m->values.rows + row_i;
        for(col_i = 0; col_i < m->ncols; col_i++) {
            sum = 0;

            for(col_j = 0; col_j < a->ncols; col_j++)
                sum += a->values.rows[row_i].values.literals[col_j] * b->values.rows[col_j].values.literals[col_i];

            row->values.literals[col_i] = sum;
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
        for(col_i = 0; col_i < m->ncols - 1; col_i++)
            printf("%.2f\t", m->values.literals[col_i]);
        printf("%.2f\n", m->values.literals[col_i]);
    } else {
        for(row_i = 0; row_i < m->nrows; row_i++) {
            row = m->values.rows + row_i;
            putchar('\t');
            for(col_i = 0; col_i < row->ncols - 1; col_i++)
                printf("%.2f\t", row->values.literals[col_i]);
            printf("%.2f\n", row->values.literals[col_i]);
        }
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
            rcopy = copy->values.rows + row_i;
            rcopy->values.literals = malloc(copy->ncols * sizeof(double));
            for(col_i = 0; col_i < m->ncols; col_i++)
                copy->values.rows[row_i].values.literals[col_i] = m->values.rows[row_i].values.literals[col_i];


        }
    return copy;
}

Matrix* try_get_matrix(Dict* matrix_dict, char** line) {
    unsigned i;
    Matrix *m;
    char matrix_identifier[MAXIDENTIFIER];
    char *ptr = *line;

    for(i = 0; i < MAXIDENTIFIER - 1 && isalnum(*ptr); i++)
       matrix_identifier[i] = *ptr++;
    matrix_identifier[i] = '\0';

    /* matrix does not exist */
    if((m = dict_get(matrix_dict, matrix_identifier)) == NULL) {
       printf("Error: matrix %s does not exist.\n", matrix_identifier);
       return NULL;
    }

    *line = ptr;
    return m;
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
            row = m->values.rows + row_i;
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
            row = m->values.rows + row_i;
            vals = malloc(ncols * sizeof(double));
            for(col_i = 0; col_i < ncols; col_i++)
                vals[col_i] = row->values.literals[col + col_i];
            free(row->values.literals);
            row->values.literals = vals;
            row->ncols = ncols;
        }
        m->ncols = ncols;
    }
}

Matrix* matrix_multiply_constant(Matrix* m, double val) {
    unsigned col_i, row_i;
    Matrix* row;
    Matrix* copy = matrix_copy(m);
    for(row_i = 0; row_i < copy->nrows; row_i++) {
        row = copy->values.rows + row_i;
        for(col_i = 0; col_i < copy->ncols; col_i++)
            row->values.literals[col_i] *= val;
    }

    return copy;
}
