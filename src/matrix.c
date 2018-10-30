#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "defs.h"
#include "matrix.h"
#include "dict.h"

/*
 * internal_matrix represented as a 2D array
 */
struct Row {
    unsigned len;
    int *vals;
};

void row_destroy(Row *r) {
    free(r->vals);
}

unsigned check_row_size(Row* ptr, unsigned size) {
    int* vals;

    if(ptr->len < size) {
        return size;
    }

    size *= 2;
    if((vals = realloc(ptr->vals, size * sizeof(int))) == NULL) {
        printf("Error: could not allocate memory. Try again.\n");
        row_destroy(ptr);
        free(ptr);
        return 0;
    }
    ptr->vals = vals;

    return size;
}

Row* row_parse(char** line) {
    char c, *ptr = *line;
    int dig = 0, *vals;
    unsigned size = 1;

    Row* row = malloc(sizeof(Row));
    row->len = 0;
    row->vals = malloc(size * sizeof(int));

    if((c = *ptr++) != '[') {
        printf("Error: Row must start with a [\n");
        row_destroy(row);
        free(row);
        return NULL;
    }

    while((c = *ptr++) != '\0') {
        if(c == ']') {
            if((size = check_row_size(row, size)) == 0) {
                return NULL;
            }
            row->vals[row->len++] = dig;
            break;
        } else if(c == ',') {
            if((size = check_row_size(row, size)) == 0) {
                return NULL;
            }
            row->vals[row->len++] = dig;
            dig = 0;
        } else if(isdigit(c)) {
            dig = dig * 10 + c - '0';
        } else if(!isspace(c)) {
            printf("Error: Invalid character in matrix defintion : %c\n", c);
            row_destroy(row);
            free(row);
            return NULL;
        }
    }

    if(c == '\0') {
        row_destroy(row);
        free(row);
        return NULL;
    }

    if((vals = realloc(row->vals, row->len * sizeof(int))) == NULL) {
        printf("Error: could not allocate memory. Try again.\n");
        row_destroy(row);
        free(row);
        return NULL;
    }
    row->vals = vals;
    *line = ptr;
    return row;
}

struct Matrix {
    char* name;
    unsigned nrows;
    unsigned ncols;
    Row* rows;
};

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

void matrix_destroy(Matrix* m) {
    unsigned i;
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
    else if(isdigit(*c)) {
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
        row->vals = (int*)malloc(m->ncols * sizeof(int));

        for(col_i = 0; col_i < a->ncols; col_i++) {
            row->vals[col_i] = a->rows[row_i].vals[col_i]
                             + b->rows[row_i].vals[col_i];
        }
    }

    return m;
}

Matrix* matrix_evaluate(char* identifier, char* line) {
    char id[MAXIDENTIFIER];
    unsigned short i = 0, id_i = 0;
    Matrix* matrix;
    Matrix** matrices = (Matrix**)malloc(2*sizeof(Matrix*));

    struct {
        unsigned int addition : 1;
    } flags = {
        0
    };

    // TODO 
    // C = A + B
    // C = A * B
    // C = A - B

    while(*line != '\n' && *line != '\0') {
        while(*line == ' ')
            line++;

        if(*line == '+') {
            line++;
            flags.addition = 1;
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

        if((matrix = dict_get(id)) == NULL) {
            printf("Error: invalid matrix identifier\n");
            free(matrices);
            return NULL;
        }
        matrices[i++] = matrix;
    }

    if(flags.addition)
        matrix = matrix_add(identifier, matrices[0], matrices[1]);
    else {
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
            printf("%d\t", row->vals[j]);
        }
        printf("%d\n", row->vals[row->len-1]);
    }
    printf("]\n");
}
