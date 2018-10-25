#ifndef DICT_H
#define DICT_H

#include "matrix.h"

typedef struct nlist* nlist;

#define HASHSIZE 101
static nlist dict[HASHSIZE];

Matrix dict_get(char* key);
void *dict_add(char* key, Matrix val);
Matrix dict_remove(char* key);

#endif
