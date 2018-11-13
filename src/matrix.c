#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "matrix.h"

/*
 * Returns rows by cols Matrix without a name
 */
Matrix* matrix_create(char* identifier) {
    Matrix* matrix;

    matrix = (Matrix*)malloc(sizeof(Matrix));

    matrix->rows = NULL;
    matrix->name = strdup(identifier);
    matrix->nrows = 0;
    matrix->ncols = 0;

    return matrix;
}

Matrix* matrix_create_dim(char* identifier, unsigned nrows, unsigned ncols) {
    Matrix* m = matrix_create(identifier);
    m->nrows = nrows;
    m->ncols = ncols;
    return m;
}

Matrix* matrix_create_empty(char* identifier, unsigned nrows, unsigned ncols) {
    unsigned row_i;
    Row* r;
    Matrix* m = matrix_create(identifier);
    m->nrows = nrows;
    m->ncols = ncols;
    m->rows = malloc(nrows * sizeof(struct Row));

    for(row_i = 0; row_i < nrows; row_i++) {
        r = m->rows + row_i;
        r->len = ncols;
        r->pivot = ncols;
        r->vals = malloc(ncols * sizeof(double));
    }

    return m;
}

Matrix* matrix_create_zero(char* identifier, unsigned nrows, unsigned ncols) {
    unsigned row_i, col_i;
    Matrix* m = matrix_create_empty(identifier, nrows, ncols);
    for(row_i = 0; row_i < m->nrows; row_i++)
        for(col_i = 0; col_i < m->ncols; col_i++)
            m->rows[row_i].vals[col_i] = 0;

    return m;
}

void matrix_destroy(void* data) {
    unsigned i;
    Matrix* m = (Matrix*)data;

    if(m->rows != NULL) {
        for(i = 0; i < m->nrows; i++)
            row_destroy((m->rows + i));
        free(m->rows);
    }
    free(m->name);
    free(m);
}

Matrix* matrix_parse(char* identifier, char* line) {
    unsigned row_check = 0, size = 1;
    char *c = line;
    Row *r, *rows;
    Matrix* matrix = matrix_create(identifier);

    matrix->rows = (Row*)malloc(sizeof(Row));
    matrix->nrows = 0;
    matrix->ncols = 0;

    if(*c++ != '[') {
        printf("Error: matrix must start with a [\n");
        matrix_destroy(matrix);
        return NULL;
    }

    while(*c == ' ')
        c++;

    if(*c == '[') {
        do {
            if((r = row_parse(&c)) == NULL) {
                matrix_destroy(matrix);
                return NULL;
            }

            if(row_check == 0)
                row_check = r->len;
            else if(r->len != row_check) {
                printf("Error: matrix rows must have same length\n");
                matrix_destroy(matrix);
                row_destroy(r);
                free(r);
                return NULL;
            }

            if(matrix->nrows >= size) {
                size *= 2;
                if((rows = realloc(matrix->rows, size * sizeof(struct Row))) == NULL) {
                    printf("Error: could not allocate memory. Try again.\n");
                    matrix_destroy(matrix);
                    row_destroy(r);
                    free(r);
                    return NULL;
                }
                matrix->rows = rows;
            }
            matrix->rows[matrix->nrows++] = *r;

            /* skip spaces, commas, and deal with end of matrix */
            while(*c == ' ')
                c++;
            if(*c == ']')
                break;
            if(*c == ',')
                c++;
            while(*c == ' ')
                c++;
        } while(*c == '[');
    }
    else if(isdigit(*c) || *c == '-') {
        /* this matrix is just a vector */
        if((r = row_parse(&line)) == NULL) {
            matrix_destroy(matrix);
            return NULL;
        } else {
            matrix->rows[0] = *r;
            matrix->nrows = 1;
            row_check = r->len;
        }
    } else {
        printf("Error: Invalid character in matrix: %c\n", *line);
        matrix_destroy(matrix);
        return NULL;
    }

    if(matrix->nrows < size) {
        if((rows = realloc(matrix->rows, matrix->nrows * sizeof(struct Row))) == NULL) {
            printf("Error: could not allocate memory. Try again.\n");
            matrix_destroy(matrix);
            return NULL;
        }
        matrix->rows = rows;
    }

    matrix->ncols = row_check;

    return matrix;
}

Matrix* matrix_add(char* identifier, Matrix* a, Matrix* b) {
    Matrix* m;
    Row* row;
    unsigned row_i, col_i;

    if(a->ncols != b->ncols || a->nrows != b->nrows) {
        printf("Error: matrices have different dimensions\n");
        return NULL;
    }

    m = matrix_create(identifier);
    m->rows = (Row*)malloc(a->nrows * sizeof(Row));
    m->nrows = a->nrows;
    m->ncols = a->ncols;

    for(row_i = 0; row_i < a->nrows; row_i++) {
        row = m->rows + row_i;
        row->len = m->ncols;
        row->vals = (double*)malloc(m->ncols * sizeof(double));

        for(col_i = 0; col_i < a->ncols; col_i++) {
            row->vals[col_i] = a->rows[row_i].vals[col_i]
                             + b->rows[row_i].vals[col_i];
        }
    }

    return m;
}

Matrix* matrix_subtract(char* identifier, Matrix* a, Matrix* b) {
    Matrix* m;
    Row* row;
    unsigned row_i, col_i;

    if(a->ncols != b->ncols || a->nrows != b->nrows) {
        printf("Error: matrices have different dimensions\n");
        return NULL;
    }

    m = matrix_create(identifier);
    m->rows = (Row*)malloc(a->nrows * sizeof(Row));
    m->nrows = a->nrows;
    m->ncols = a->ncols;

    for(row_i = 0; row_i < a->nrows; row_i++) {
        row = m->rows + row_i;
        row->len = m->ncols;
        row->vals = (double*)malloc(m->ncols * sizeof(double));

        for(col_i = 0; col_i < a->ncols; col_i++) {
            row->vals[col_i] = a->rows[row_i].vals[col_i]
                             - b->rows[row_i].vals[col_i];
        }
    }

    return a;
}


Matrix* matrix_multiply(char* identifier, Matrix* a, Matrix* b) {
    Matrix* m;
    Row* row;
    unsigned row_i, col_i, col_j;
    int sum;

    if(a->ncols != b->nrows) {
        printf("Error: matrices cannot be multiplied\n");
        return NULL;
    }

    m = matrix_create(identifier);
    m->rows = (Row*)malloc(a->nrows * sizeof(Row));

    /* dimensions: a=mxn b=nxp matrix=mxp */
    m->nrows = a->nrows;
    m->ncols = b->ncols;

    for(row_i = 0; row_i < a->nrows; row_i++) {
        row = m->rows + row_i;
        row->len = m->ncols;
        row->vals = (double*)malloc(m->ncols * sizeof(double));

        for(col_i = 0; col_i < m->ncols; col_i++) {
            sum = 0;

            for(col_j = 0; col_j < a->ncols; col_j++)
                sum += a->rows[row_i].vals[col_j] * b->rows[col_j].vals[col_i];

            row->vals[col_i] = sum;
        }
    }

    return m;
}

Matrix* matrix_evaluate(Dict* d, char* identifier, char* line) {
    char id[MAXIDENTIFIER];
    unsigned short i = 0, id_i = 0;
    Matrix* matrix;
    Matrix** matrices = (Matrix**)malloc(2*sizeof(Matrix*));

    struct {
        unsigned int addition : 1;
        unsigned int mult : 1;
    } flags = {
        0, 0
    };

    while(*line != '\n' && *line != '\0') {
        while(*line == ' ')
            line++;

        if(*line == '+') {
            line++;
            flags.addition = 1;
            continue;
        }

        if(*line == '*') {
            line++;
            flags.mult = 1;
            continue;
        }

        if(!isalnum(*line)) {
            printf("Error: invalid character %c\n", *line);
            free(matrices);
            return NULL;
        }

        id_i = 0;
        do
            id[id_i++] = *line++;
        while(isalnum(*line) && id_i < MAXIDENTIFIER - 1);

        if(id_i == MAXIDENTIFIER - 1) {
            printf("Error: matrix identifiers can only be %d long\n", MAXIDENTIFIER -1);
            free(matrices);
            return NULL;
        }

        id[id_i] = '\0';

        if((matrix = dict_get(d, id)) == NULL) {
            printf("Error: invalid matrix identifier\n");
            free(matrices);
            return NULL;
        }
        matrices[i++] = matrix;
    }

    if(flags.addition)
        matrix = matrix_add(identifier, matrices[0], matrices[1]);
    else if(flags.mult) {
        matrix = matrix_multiply(identifier, matrices[0], matrices[1]);
    } else {
        free(matrices);
        return NULL;
    }

    return matrix;
}

void matrix_print(Matrix* m) {
    unsigned i, j;
    Row* row;

    if(m == NULL) {
        printf("Error: cannot print matrix. Matrix does not exist.\n");
        return;
    }

    printf("%s\n", m->name);
    printf("[\n");
    for(i = 0; i < m->nrows; i++) {
        row = m->rows + i;
        putchar('\t');
        for(j = 0; j < row->len - 1; j++) {
            printf("%.2f\t", row->vals[j]);
        }
        printf("%.2f\n", row->vals[row->len-1]);
    }
    printf("]\n");
}

/* deep copies the matrix. if identifier is null, it uses the name
 * of the original matrix.
 */
Matrix* matrix_copy(Matrix* m, char* identifier) {
    Matrix* copy;
    Row* rcopy;
    unsigned i;

    if(identifier == NULL)
        identifier = m->name;

    copy = matrix_create(identifier);
    copy->ncols = m->ncols;
    copy->nrows = m->nrows;
    copy->name = strdup(identifier);
    copy->rows = malloc(sizeof(Row) * copy->nrows);

    for(i = 0; i < m->nrows; i++)
        if((rcopy = row_copy(m->rows + i)) == NULL) {
            printf("Error: could not copy row\n");
            matrix_destroy(copy);
            return NULL;
        } else
            copy->rows[i] = *rcopy;
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
    unsigned i;
    Row* row;
    double* vals;

    for(i = 0; i < m->nrows; i++) {
        row = m->rows + i;
        if((vals = realloc(row->vals, col * sizeof(double))) == NULL) {
            printf("Error: could not allocate memory. Try again.\n");
            continue;
        }
        row->vals = vals;
        row->len = col;
    }
    m->ncols = col;
}

void matrix_slice_after(Matrix *m, unsigned col) {
    unsigned row_i, col_i, ncols = m->ncols - col;

    Row *row;
    double* vals;
    for(row_i = 0; row_i < m->nrows; row_i++) {
        row =  m->rows + row_i;
        vals = malloc(ncols * sizeof(double));
        for(col_i = 0; col_i < ncols; col_i++)
            vals[col_i] = row->vals[col + col_i];
        free(row->vals);
        row->vals = vals;
        row->len = ncols;
        row->pivot = ncols;
    }
    m->ncols = ncols;
}

void matrix_multiply_constant(Matrix* m, double val) {
    unsigned col_i, row_i;
    Row* row;
    for(row_i = 0; row_i < m->nrows; row_i++) {
        row = m->rows + row_i;
        for(col_i = 0; col_i < m->ncols; col_i++)
            row->vals[col_i] *= val;
    }
}
