#ifndef I_H
#define I_H

#include "matrix.h"

void I_handler(Dict* matrix_dict, char* line);
Matrix* try_I(Dict* matrix_dict, unsigned nrows, char* identifier);
Matrix* I(unsigned nrows, char* identifier);

#endif
