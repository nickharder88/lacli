#ifndef DISC_H
#define DISC_H

#include "../rval.h"

Rval* discriminant_handler(Rval** args, unsigned nargs);
Rval* discriminant(Matrix* m);
double discriminant_solve(double m_trace, double m_det);

#endif
