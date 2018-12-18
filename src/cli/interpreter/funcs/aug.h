#ifndef AUG_H
#define AUG_H

#include "../rval.h"

Rval* aug_handler(Rval** args, unsigned nargs);
Rval* aug(Matrix** marr, unsigned nmatrices);

#endif
