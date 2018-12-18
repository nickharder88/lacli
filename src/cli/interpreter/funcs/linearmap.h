#ifndef LINEARMAP_H
#define LINEARMAP_H

#include "../rval.h"

Rval* linearmap_handler(Rval** args, unsigned nargs);
Rval* linearmap(Matrix** vrows, Matrix** wrows, unsigned nrow_vectors);

#endif
