#ifndef AUG_H
#define AUG_H

#include "matrix.h"

void aug_handler(Dict* matrix_dict, char* line);
Matrix* try_aug(Dict* matrix_dict, Matrix* m1, Matrix* m2, char* identifier);
Matrix* aug(Matrix* m1, Matrix* m2, char* identifier);

#endif
