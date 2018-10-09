#include <stdio.h>

#define MAXLEN_NAME 24

typedef struct Matrix* Matrix;

Matrix matrix_parse(char* line);

Matrix matrix_create(char* name, int rows, int cols);

int matrix_change(Matrix m, int row, int col, int val);

void matrix_print(Matrix m, FILE* ofile);

void matrix_destroy(Matrix m);
