#ifndef EVEC_H
#define EVEC_H

#include "../rval.h"

Rval* evec_handler(Rval** args, unsigned nargs);
Rval* evec(Matrix* m);
Rval* evec_eig(Matrix* m, Rval* evals);

#endif
