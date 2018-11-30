#ifndef AUG_H
#define AUG_H

#include "../rval.h"

Rval* aug_handler(Rval* args, unsigned nargs);
Rval* aug(Matrix* m1, Matrix* m2);

#endif
