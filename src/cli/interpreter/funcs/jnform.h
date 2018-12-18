#ifndef JNFORM_H
#define JNFORM_H

#include "../rval.h"

Rval* jnform_handler(Rval** args, unsigned nargs);
Rval* jnform(Matrix* m);
Rval* jnform_eval(Matrix *m, Rval* evals);

#endif
