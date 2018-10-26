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
typedef struct Row {
    unsigned len;
    int *vals;
} Row;

void row_destroy(Row *r) {
    free(r->vals);
    free(r);
}

// A = [[1,2,3],[4,5,6]]
Row* row_parse(char* line) {
    char c;
    unsigned len = 0;
    int *vals, dig = 0, size = 1;

    Row* row = (Row*) malloc(sizeof(Row));
    row->len = 0;
    vals = row->vals = (int*)malloc(size * sizeof(int));

    if((c = *line++) != '[') {
        printf("Error: Row must start with a [\n");
        row_destroy(row);
        return NULL;
    }

    while((c = *line++) != '\0') {
        if(c == ']') {
            vals[len++] = dig;
            break;
        } else if(c == ',') {
            vals[len++] = dig;
            if(len >= size) {
                size *= 2;
                vals = (int *)realloc(vals, size);
            }
            dig = 0;
        } else if(isdigit(c)) {
            dig = dig * 10 + c - '0';
        } else if(!isspace(c)) {
            printf("Error: Invalid character in matrix defintion : %c\n", c);
            row_destroy(row);
            return NULL;
        }
    }

    if(c == '\0') {
        row_destroy(row);
        return NULL;
    }

    vals = (int*)realloc(vals, len);
    row->len = len;
    return row;
}

struct Matrix {
    char* name;
    unsigned nrows;
    unsigned ncols;
    Row* rows;
};
typedef struct Matrix* Matrix;

/*
 * Returns rows by cols Matrix without a name
 */
Matrix matrix_create(char* identifier) {
    Matrix matrix;

    matrix = (Matrix)malloc(sizeof(struct Matrix));

    matrix->rows = NULL;
    matrix->name = strdup(identifier);
    matrix->nrows = 0;
    matrix->ncols = 0;

    return matrix;
}

void matrix_destroy(Matrix m) {
    if(m->rows != NULL)
        free(m->rows);
    free(m->name);
    free(m);
}

Matrix matrix_parse(char* identifier, char* line) {
    char *c = line;
    Row *r;
    Matrix matrix = matrix_create(identifier);

    if(*c++ != '[') {
        printf("Error: matrix must start with a [\n");
        matrix_destroy(matrix);
        return NULL;
    }

    while(*c == ' ')
        c++;

    if(*c == '[')
        row_parse(c);
    else if(isdigit(*c)) {
        /* this matrix is just a vector */
        if((r = row_parse(line)) == NULL) {
            matrix_destroy(matrix);
            return NULL;
        } else {
            matrix->rows = (Row*) malloc(sizeof(Row));
            matrix->rows[0] = *r;
        }
    } else {
        printf("Error: Invalid character in matrix: %c\n", *line);
        matrix_destroy(matrix);
        return NULL;
    }

    matrix->ncols = r->len;
    matrix->nrows = 1;

    return matrix;
}

Matrix matrix_add(char* identifier, Matrix a, Matrix b) {
    Matrix m;
    Row *row;
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
        row = (Row*) malloc(sizeof(Row));
        row->len = m->ncols;
        row->vals = (int*)malloc(m->ncols * sizeof(int));

        for(col_i = 0; col_i < a->ncols; col_i++) {
            row->vals[col_i] = a->rows[row_i].vals[col_i]
                             + b->rows[row_i].vals[col_i];
        }

        m->rows[row_i] = *row;
    }

    return m;
}

Matrix matrix_evaluate(char* identifier, char* line) {
    char id[MAXIDENTIFIER];
    unsigned short i = 0, id_i = 0;
    Matrix matrix;
    Matrix *matrices = (Matrix*)malloc(2*sizeof(Matrix));

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

void matrix_print(Matrix m) {
    unsigned i, j;
    Row row;

    if(m == NULL) {
        printf("Error: cannot print matrix. Matrix does not exist.\n");
        return;
    }

    printf("%s\n", m->name);
    printf("[\n");
    for(i = 0; i < m->nrows; i++) {
        row = m->rows[i];
        putchar('\t');
        for(j = 0; j < row.len - 1; j++) {
            printf("%d\t", row.vals[j]);
        }
        printf("%d\n", row.vals[row.len-1]);
    }
    printf("]\n");
}
