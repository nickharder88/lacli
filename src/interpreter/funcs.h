#ifndef FUNCS_H
#define FUNCS_H

#include "rval.h"

void funcs_init(void);
void funcs_destroy(void);
Rval* func_call(char* name, Rval** args, unsigned nargs);

#endif
