#ifndef DICT_H
#define DICT_H

#include "matrix.h"

Matrix* dict_get(char* key);
void *dict_add(char* key, Matrix* val);
Matrix* dict_remove(char* key);

void dict_iter_begin(void);
Matrix* dict_next(void);

#endif
