#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "evaluate.h"
#include "rval.h"

void env_init(void);
void env_destroy(void);
void env_define(char* identifier, Rval* val);
Rval* env_get(char* identifier);

#endif
