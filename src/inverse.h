#ifndef INVERSE_H
#define INVERSE_H

#include "matrix.h"

void inverse_handler(Dict* matrix_dict, char* line);
Matrix* try_inverse(Dict* matrix_dict, Matrix* m, char* identifier);
Matrix* inverse(Matrix* m, char* identifier);

#endif
