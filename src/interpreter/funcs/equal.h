#ifndef EQUAL_H
#define EQUAL_H

#include "../rval.h"

Rval* equal_handler(Rval** args, unsigned nargs);
Rval* equal(Rval* val1, Rval* val2);

#endif
