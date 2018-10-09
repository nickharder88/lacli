#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "matrix.h"

/*
 * internal_matrix represented as a 2D array
 */
struct Matrix {
    char name[MAXLEN_NAME];
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

    matrix = (Matrix)malloc(sizeof(struct Matrix));

    internal_matrix = (int**)malloc(rows * sizeof(int *));
    matrix->internal_matrix = internal_matrix;
    strncpy(matrix->name, name, MAXLEN_NAME - 1);
    matrix->name[MAXLEN_NAME - 1] = '\0';

    /*
     * fill each row with columns(with values of 0)
     */
    for(i = 0; i < rows; i++)
        internal_matrix[i] = (int*)calloc(cols, sizeof(int));

    return matrix;
}

void matrix_change(Matrix m, int row, int col, int val) {
    m->internal_matrix[row][col] = val;
}

void matrix_print(Matrix m, FILE* ptr) {
    int row, col;

    fprintf(ptr, "%s %d X %d\n", m->name, m->rows, m->cols);
    for(row = 0; row < m->rows; row++) {
        for(col = 0; col < m->cols - 1; col++) {
            /*
             * TODO make this pretty print
             */
            fprintf(ptr, "%d ", m->internal_matrix[row][col]);
        }
        fprintf(ptr, "%d\n", m->internal_matrix[m->rows - 1, m->cols - 1]);
    }
    fprintf(ptr, "\n");
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
