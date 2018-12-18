#ifndef LININD_H
#define LININD_H

#include "../rval.h"

Rval* linind_handler(Rval** args, unsigned nargs);
Rval* linind(Matrix** col_vectors, unsigned ncols);

#endif
