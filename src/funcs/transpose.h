#ifndef TRANSPOSE_H
#define TRANSPOSE_H

#include "matrix.h"

void transpose_handler(Dict* matrix_dict, char* line);
Matrix* try_transpose(Dict* matrix_dict, Matrix* m, char* identifier);
Matrix* transpose(Matrix* m, char* identifier);

#endif
