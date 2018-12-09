#ifndef RVAL_H
#define RVAL_H

#include "../matrix.h"
#include "equation.h"
#include "array.h"
#include "cplx.h"

typedef enum {
    TRUE, FALSE
} Boolean;

typedef struct Rval {
    enum {
        RMATRIX, RMATRIX_ARRAY, RLITERAL, RLITERAL_ARRAY, RNIL, RBOOLEAN, REQU, RCPLX, RCPLX_ARRAY
    } type;

    union {
        double literal;
        Boolean boolean;

        Array* array;
        Matrix* matrix;
        Equation* equation;
        Cplx* cplx;
    } value;
} Rval;

void rval_destroy(Rval* val);
void rval_print(Rval* val);
char rval_cmp(Rval* val1, Rval* val2);

Rval* rval_make_literal(double val);
Rval* rval_make_matrix(Matrix* m);
Rval* rval_make_nil(void);
Rval* rval_make_literal_array(double* vals, unsigned length);
Rval* rval_make_boolean(Boolean boolean);
Rval* rval_make_matrix_array(Matrix** marr, unsigned length);
Rval* rval_make_equ(Equation* equ);
Rval* rval_make_cplx(Cplx* cplx);
Rval* rval_make_cplx_array(Cplx** cplxs, unsigned length);

#endif
