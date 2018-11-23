#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include "defs.h"
#include "dict.h"
#include "row.h"

typedef struct Matrix {
    unsigned nrows;
    unsigned ncols;
    Row* rows;
} Matrix;

Matrix* matrix_create();
Matrix* matrix_create_dim(unsigned nrows, unsigned ncols);
Matrix* matrix_create_empty(unsigned nrows, unsigned ncols);
Matrix* matrix_create_zero(unsigned nrows, unsigned ncols);
void matrix_destroy(void* m);

Matrix* matrix_parse(char* line);
Matrix* matrix_evaluate(Dict* d, char* line);
void matrix_print(Matrix* m);

void matrix_multiply_constant(Matrix* m, double val);
Matrix* matrix_subtract(Matrix *m1, Matrix *m2);

Matrix* matrix_copy(Matrix* m);
Matrix* try_get_matrix(Dict* matrix_dict, char** line);

void matrix_slice_before(Matrix *m, unsigned col);
void matrix_slice_after(Matrix *m, unsigned col);

#endif
