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
    int *vals, dig, size = 1;

    Row row = (Row) malloc(sizeof(Row));
    row->len = 0;
    vals = row->vals = (int*)malloc(size * sizeof(int));

    if((c = getchar()) != '[') {
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

/*
 * Returns rows by cols Matrix without a name
 */
Matrix matrix_create(char* identifier) {
    int i;
    int* row;
    Matrix matrix;

    matrix = (Matrix)malloc(sizeof(struct Matrix));

    matrix->rows = NULL;
    matrix->name = strdup(identifier);
    matrix->nrows = USHRT_MAX;
    matrix->ncols = USHRT_MAX;

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

    while(*c == ' ')
        c++;

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
        } else
            matrix->rows = &r;
    } else {
        printf("Error: Invalid character in matrix: %c\n", *line);
        matrix_destroy(matrix);
        return NULL;
    }

    return matrix;
}

struct Matrix {
    char* name;
    unsigned nrows;
    unsigned ncols;
    Row* rows;
};


void matrix_print(Matrix m, FILE* ofile) {
    unsigned i;
    Row r;

    if(m == NULL) {
        printf("Error: cannot print matrix. Matrix does not exist.\n");
        return;
    }

    fprintf(ofile, "%s:", m->name);

    for(i = 0; i < m->nrows; i++) {
        // TODO finish printing matrixes
    }
}









