#ifndef ISSTABLE_H
#define ISSTABLE_H

#include "../rval.h"

Rval* isstable_handler(Rval** args, unsigned nargs);
Rval* isstable(Matrix* m);

#endif
