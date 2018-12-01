#ifndef RREF_H
#define RREF_H

#include "../rval.h"

unsigned get_pivot(Matrix* row);
Rval* rref_handler(Rval** args, unsigned nargs);
Rval* rref(Matrix* m);

#endif
