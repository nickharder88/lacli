#ifndef EVAL_H
#define EVAL_H

#include "matrix.h"

Rval* eval_handler(Rval* args, unsigned nargs);
char eval(Matrix* m, double* eval1, double* eval2);

#endif
