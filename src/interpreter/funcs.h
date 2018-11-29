#ifndef FUNCS_H
#define FUNCS_H

#include "evaluate.h"
#include "rval.h"

void funcs_init(void);
Rval* func_call(char* name, Rval* args);

#endif
