#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include "defs.h"

typedef struct Row Row;
typedef struct Matrix Matrix;

Matrix* matrix_create(char* identifier);
void matrix_destroy(Matrix* m);

Matrix* matrix_parse(char* identifier, char* line);
Matrix* matrix_evaluate(char* identifier, char* line);
int matrix_change(Matrix* m, int row, int col, int val);

void matrix_print(Matrix* m);

#endif
