#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include "defs.h"
#include "dict.h"

typedef struct Matrix {
    unsigned nrows;
    unsigned ncols;

    unsigned char dim;

    union {
        struct Matrix* rows;
        double* literals;
    } values;
} Matrix;

Matrix* matrix_create();
Matrix* matrix_create_dim(unsigned nrows, unsigned ncols);
Matrix* matrix_create_zero(unsigned nrows, unsigned ncols);
void matrix_destroy(void* m);

Matrix* matrix_parse(char* line);
Matrix* matrix_evaluate(Dict* d, char* line);
void matrix_print(Matrix* m);


Matrix* matrix_copy(Matrix* m);
Matrix* try_get_matrix(Dict* matrix_dict, char** line);

Matrix* matrix_add(Matrix* a, Matrix* b);
Matrix* matrix_subtract(Matrix* a, Matrix* b);
Matrix* matrix_multiply_constant(Matrix* m, double val);
Matrix* matrix_multiply(Matrix* a, Matrix* b);
Matrix* matrix_subtract(Matrix *m1, Matrix *m2);

void matrix_slice_before(Matrix *m, unsigned col);
void matrix_slice_after(Matrix *m, unsigned col);

#endif
