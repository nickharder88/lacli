#ifndef ISSIMILAR_H
#define ISSIMILAR_H

#include "../rval.h"

Rval* issimilar_handler(Rval** args, unsigned nargs);
Rval* issimilar(Matrix* m1, Matrix* m2);

#endif
