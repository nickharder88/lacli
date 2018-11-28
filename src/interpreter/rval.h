#ifndef RVAL_H
#define RVAL_H

#include "../matrix.h"

typedef struct Rval {
    enum {
        RMATRIX, RLITERAL
    } type;

    union {
        double literal;
        Matrix* matrix;
    } value;
} Rval;

void rval_destroy(Rval* val);
void rval_print(Rval* val);

#endif
