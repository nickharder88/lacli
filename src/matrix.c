#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "defs.h"
#include "matrix.h"

/*
 * internal_matrix represented as a 2D array
 */
typedef struct Row* Row;
struct Row {
    unsigned len;
    int *vals;
};

void row_destroy(Row r) {
    free(r->vals);
    free(r);
}

// A = [[1,2,3],[4,5,6]]
Row row_parse(char* line) {
    char c;
    unsigned len = 0;
    int *vals, dig = 0, size = 1;

    Row row = (Row) malloc(sizeof(Row));
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
    Row rows;
};

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
    Row r;
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
            matrix->rows = (Row) malloc(sizeof(struct Row));
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

Matrix matrix_evaluate(char* identifier, char* line) {
    Matrix matrix;

    // TODO 
    // C = A + B
    // C = A * B
    // C = A - B
    
    // gets the variables and adds them together.
    // first support 2 matrix, then infinite?
    

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
        row = m->rows + i;
        putchar('\t');
        for(j = 0; j < row->len - 1; j++) {
            printf("%d\t", row->vals[j]);
        }
        printf("%d\n", row->vals[row->len-1]);
    }
    printf("]\n");
}
