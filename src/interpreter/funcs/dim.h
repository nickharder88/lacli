#ifndef DIM_H
#define DIM_H

#include "../rval.h"

Rval* dim_handler(Rval** args, unsigned nargs);
Rval* dim(Matrix** cols, unsigned ncols);

#endif
