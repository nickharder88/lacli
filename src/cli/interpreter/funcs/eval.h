#ifndef EVAL_H
#define EVAL_H

#include "../rval.h"

Rval* eval_handler(Rval** args, unsigned nargs);
Rval* eval(Matrix* m);

#endif
