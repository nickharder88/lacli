#ifndef FUNCS_H
#define FUNCS_H

#include "matrix.h"

void func_destroy(void* func);
Dict* func_create(void);

/* expects the entire user input */
void* is_func(Dict* func_dict, char* line);
void call_func(void* func_dict, Dict* matrix_dict, char* line);

Matrix** parse_args(Dict* matrix_dict, char* line, unsigned nmatrices, char** identifier);

#endif
