#ifndef RVAL_H
#define RVAL_H

#include "../matrix.h"

typedef struct Rval {
    enum {
        RMATRIX, RLITERAL, RLITERAL_ARRAY, RNIL
    } type;

    union {
        struct {
            double* literal_array;
            unsigned length;
        } array;

        double literal;
        Matrix* matrix;
    } value;
} Rval;

void rval_destroy(Rval* val);
void rval_print(Rval* val);
Rval* rval_make_literal(double val);
Rval* rval_make_matrix(Matrix* m);
Rval* rval_make_nil(void);
Rval* rval_make_literal_array(double* vals, unsigned length);

#endif
