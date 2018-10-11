#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "defs.h"
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
Matrix matrix_create(void) {
    int i;
    int* row;
    Matrix matrix;

    matrix = (Matrix)malloc(sizeof(struct Matrix));

    matrix->internal_matrix = NULL;
    matrix->name = NULL;
    matrix->rows = 0;
    matrix->cols = 0;

    return matrix;
}

int* get_row(char** line, int* nelements) {
    int val;
    unsigned i, size;
    int* row = (int*)malloc(sizeof(int));
    char *ptr = *line;

    if(*(ptr++) != '[') {
        printf("Error: Row must start with [\n");
        *nelements = 0;
        free(row);
        return NULL;
    }

    for(i = 0; 1; i++) {
        if(i == size) {
            size *= 2;
            row = (int*) realloc(row, size);
        }

        while(isspace(*ptr))
            ptr++;

        for(val = 0; isdigit(*ptr); ptr++)
            val = val * 10 + *ptr - '0';
        row[i] = val;

        while(isspace(*ptr))
            line++;

        if(*ptr == ',')
            ptr++;
        else if(*ptr == ']')
            break;
        else {
            printf("Error: Invalid character %c\n", *line);
            *nelements = 0;
            free(row);
            return NULL;
        }
    }

    *nelements = i;
    *line = ptr;
    return row;
} 

/*
 * Return a matrix from a line
 */
Matrix matrix_parse(char* identifier, char* line) {
    char i;
    char *ptr, *name;

    int val, rows, cols;

    Matrix matrix;

    /* skip the colon since we already parsed it */
    matrix = matrix_create();
    if(matrix == NULL)
        return NULL;

    if(line == NULL) {
        printf("Error: tried to parse empty line into matrix.\n");
        return NULL;
    }

    if(*(line++) != '[') {
        printf("Error: Invalid matrix syntax. Must start with [\n");
        return NULL;
    }

    /* TODO table double the rows */

    for(row = 0; row < matrix->rows; row++) {
        if(*(line++) != '[') {
            printf("Error: Invalid matrix syntax. Rows must start with [\n");
            return EXIT_FAILURE;
        }
        offset++;

        /* -2 for special reading of , and ] */
        for(col = 0; col < matrix->cols - 1; col++) {
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
        internal_matrix[row][col] = val;

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
        fprintf(ptr, "%d]", m->internal_matrix[row][m->cols - 1]);
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

