#ifndef DICT_H
#define DICT_H

#include "matrix.h"

typedef struct dict dict;

void* dict_create(void (*destroy)(void*));
void* dict_get(dict* d, char* key);
void *dict_add(dict* d, char* key, void* val);
void* dict_remove(dict* d, char* key);
void dict_clear(dict* d);

void dict_iter_begin(dict* d);
void* dict_next(dict* d);

#endif
