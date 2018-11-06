#ifndef DICT_H
#define DICT_H

typedef struct Dict Dict;

Dict* dict_create(void (*destroy)(void*));
void* dict_get(Dict* d, char* key);
void* dict_add(Dict* d, char* key, void* val);
void* dict_remove(Dict* d, char* key);
void dict_clear(Dict* d);

void dict_iter_begin(Dict* d);
void* dict_next(Dict* d);

#endif
