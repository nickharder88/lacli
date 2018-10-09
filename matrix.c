#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "matrix.h"

/*
 * internal_matrix represented as a 2D array
 */
struct Matrix {
    char* name;
    int rows;
    int cols;
    int** internal_matrix;
};

/*
 * Returns rows by cols Matrix without a name
 */
Matrix matrix_create(char* name, int rows, int cols) {
    int i;
    int* row;
    int** internal_matrix;
    Matrix matrix;

    if(name == NULL || rows <= 0 || cols <= 0)
        return NULL;

    matrix = (Matrix)malloc(sizeof(struct Matrix));

    internal_matrix = (int**)malloc(rows * sizeof(int *));
    matrix->internal_matrix = internal_matrix;
    matrix->name = name;
    matrix->rows = rows;
    matrix->cols = cols;

    /*
     * fill each row with columns(with values of 0)
     */
    for(i = 0; i < rows; i++)
        internal_matrix[i] = (int*)calloc(cols, sizeof(int));

    return matrix;
}

/*
 * Fills in matrix->internal_matrix with data from line.
 * Returns number of characters read(offset)
 */
unsigned matrix_internal_parse(Matrix matrix, char* line) {
    unsigned offset = 0;
    int row, col, i, val;
    int** internal_matrix = matrix->internal_matrix;

    if(line == NULL) {
        printf("Error: tried to parse empty line into matrix.\n");
        return EXIT_FAILURE;
    }

    if(*(line++) != '[') {
        printf("Error: Invalid matrix syntax. Must start with [\n");
        return EXIT_FAILURE;
    }
    offset++;

    for(row = 0; row < matrix->rows; row++) {
        if(*(line++) != '[') {
            printf("Error: Invalid matrix syntax. Rows must start with [\n");
            return EXIT_FAILURE;
        }
        offset++;

        /* -2 for special reading of , and ] */
        for(col = 0; col < matrix->cols - 2; col++) {
            for(val = 0; isdigit(*line); line++, offset++)
                val = val * 10 + ((*line) - '0');
            if(*(line++) != ',') {
                printf("Error: Invalid matrix syntax. Values must be seperated by a comma\n");
                return EXIT_FAILURE;
            }
            offset++;

            internal_matrix[row][col] = val;
        }

        for(val = 0; isdigit(*line); line++, offset++)
            val = val * 10 + ((*line) - '0');
        internal_matrix[row][col+1] = val;

        /*
         * Make sure sub matrices end with ],
         */
        if(*(line++) != ']') {
                printf("Error: Invalid matrix syntax. Row must end with ]\n");
                return EXIT_FAILURE;
        }
        offset++;

        if(row < matrix->rows - 1) {
            if(*(line++) != ',') {
                printf("Error: Invalid matrix syntax. Rows must be seperated by a comma\n");
                return EXIT_FAILURE;
            }
            offset++;
        }
    }

    if(*line != ']') {
        printf("Error: Invalid matrix syntax. Must end with ]\n");
        return EXIT_FAILURE;
    }

    return offset + 1;
}

/*
 * Return a matrix from a line
 */
Matrix matrix_parse(char* line) {
    char i;
    char *ptr, *name;
    int val, rows, cols;
    unsigned offset;
    Matrix matrix;

    name = (char*)malloc(MAXLEN_NAME * sizeof(char));

    for(i = 0, ptr = line; i < MAXLEN_NAME && *ptr != ':'; i++, ptr++)
        if(isalnum(*ptr) || *ptr == '_') {
            name[i] = *ptr;
        } else {
            printf("Error: name of matrix must only contain alphanumeric values or underscores\n");
            free(name);
            return NULL;
        }

    if(i >= MAXLEN_NAME) {
        printf("Warning: name of matrix can be a maximum of 24 characters.\n");
        i = MAXLEN_NAME - 1;

        /* skip extra garbage and go to : */
        while(*ptr != ':' && *ptr != '\n' && *ptr != '\0')
            ptr++;

        if(*ptr == '\n' || *ptr == '\0') {
            printf("Error: unexpected end of line\n");
            free(name);
            return NULL;
        }
    }
    name[i] = '\0';

    /* skip the colon since we already parsed it */
    ptr++;

    if(*(ptr++) != '(') {
        printf("Error: invalid syntax of matrix dimensions. Must start with (.\n");
        free(name);
        return NULL;
    }

    for(rows = 0; isdigit(*ptr); ptr++)
        rows = rows * 10 + (*ptr) - '0';

    if(*(ptr++) != ',') {
        printf("Error: invalid syntax of matrix dimensions. (rows,cols)\n");
        free(name);
        return NULL;
    }

    for(cols = 0; isdigit(*ptr); ptr++)
        cols = cols * 10 + (*ptr) - '0';

    if(*(ptr++) != ')') {
        printf("Error: invalid syntax of matrix dimensions. Must end with ).\n");
        free(name);
        return NULL;
    }

    if(*(ptr++) != ':') {
        printf("Error: Expected a colon after matrix dimensions. NAME:(ROWS,COLS):\n");
        free(name);
        return NULL;
    }

    matrix = matrix_create(name, rows, cols);
    if(matrix == NULL)
        return NULL;

    /* could not parse rest of line */
    if((offset = matrix_internal_parse(matrix, ptr)) == EXIT_FAILURE) {
        matrix_destroy(matrix);
        return NULL;
    }

    if(*(ptr+offset) != '\n') {
        matrix_destroy(matrix);
        return NULL;
    }

    return matrix;
}

int matrix_change(Matrix m, int row, int col, int val) {
    if(m == NULL) {
        printf("Error: null pointer in matrix_change\n");
        return EXIT_FAILURE;
    }

    if(row <= 0 || row > m->rows || col <= 0 || col > m->cols) {
        printf("Error: invalid location in matrix.\n");
        return EXIT_FAILURE;
    }

    m->internal_matrix[row][col] = val;

    return EXIT_SUCCESS;
}

/* Prints matrix to file */
void matrix_print(Matrix m, FILE* ptr) {
    if(m == NULL || ptr == NULL) {
        printf("matrix.c : matrix_print : Pointer arguments are null.\n");
        return;
    }

    int row, col;

    fprintf(ptr, "%s:(%d,%d):[", m->name, m->rows, m->cols);
    for(row = 0; row < m->rows; row++) {
        if(row > 0)
            printf(",");
        printf("[");

        for(col = 0; col < m->cols - 1; col++) {
            fprintf(ptr, "%d,", m->internal_matrix[row][col]);
        }
        fprintf(ptr, "%d]", m->internal_matrix[row, m->cols - 1]);
    }
    fprintf(ptr, "]\n");
}

void matrix_destroy(Matrix m) {
    int i;

    /*
     * free each row
     */
    for(i = 0; i < m->rows; i++) {
        free(m->internal_matrix[i]);
    }

    /*
     * free the internal representation and the matrix
     */
    free(m->internal_matrix);
    free(m);
}

