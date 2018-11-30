#ifndef TRANSPOSE_H
#define TRANSPOSE_H

#include "../rval.h"

Rval* transpose_handler(Rval* args, unsigned nargs);
Rval* transpose(Matrix* m);

#endif
