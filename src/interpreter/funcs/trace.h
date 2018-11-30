#ifndef TRACE_H
#define TRACE_H

#include "../rval.h"

Rval* trace_handler(Rval* args, unsigned nargs);
Rval* trace(Matrix* m);

#endif
