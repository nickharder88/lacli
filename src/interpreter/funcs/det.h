#ifndef DET_H
#define DET_H

#include "../rval.h"

Rval* det_handler(Rval* args, unsigned nargs);
Rval* det(Matrix* m);

#endif
