#ifndef DIFFSOLVE_H
#define DIFFSOLVE_H

#include "../rval.h"

Rval* diffsolve_handler(Rval** args, unsigned nargs);
Rval* diffsolve(Matrix *m, Matrix *init);

#endif
