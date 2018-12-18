#ifndef BASIS_H
#define BASIS_H

#include "../rval.h"

Rval* basis_handler(Rval** args, unsigned nargs);
Rval* is_basis_handler(Rval** args, unsigned nargs);

Rval* basis(Matrix** cols, unsigned ncols);
Rval* is_basis(Matrix* space, Matrix** cols, unsigned ncols);

#endif
