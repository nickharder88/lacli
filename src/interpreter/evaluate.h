#ifndef EVALUATE_H
#define EVALUATE_H

#include "expr.h"

typedef struct Rval {
    enum {
        RMATRIX, RLITERAL
    } type;

    union {
        double literal;
        Matrix* matrix;
    } value;
} Rval;

#endif
