#ifndef FUNCS_H
#define FUNCS_H

#include "dict.h"

void func_destroy(void* func);
Dict* func_create(void);

/* expects the entire user input */
void* is_func(Dict* func_dict, char* line);
void call_func(void* func_dict, char* line);

#endif

