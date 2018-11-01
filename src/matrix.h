#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include "defs.h"

typedef struct Matrix Matrix;

Matrix* matrix_create(char* identifier);
void matrix_destroy(Matrix* m);

Matrix* matrix_parse(char* identifier, char* line);
Matrix* matrix_evaluate(char* identifier, char* line);
void matrix_print(Matrix* m);

Matrix* matrix_copy(Matrix* m, char* identifier);
Matrix* rref(Matrix* m, char* identifier);
Matrix* aug(Matrix* m, Matrix* b);

#endif
