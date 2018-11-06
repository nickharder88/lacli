#ifndef RREF_H
#define RREF_H

#include "matrix.h"

void rref_handler(Dict* matrix_dict, char* line);
Matrix* try_rref(Dict* matrix_dict, Matrix* m, char* identifier);

#endif
