#ifndef RVAL_H
#define RVAL_H

#include "../matrix.h"
#include "equation.h"

typedef enum {
    TRUE, FALSE
} Boolean;

typedef struct Rval {
    enum {
        RMATRIX, RMATRIX_ARRAY, RLITERAL, RLITERAL_ARRAY, RNIL, RBOOLEAN, REQU, RSTR
    } type;

    union {
        struct {
            union {
                double* literal_array;
                Matrix** matrix_array;
            };
            unsigned length;
        } array;

        double literal;
        char* str;
        Matrix* matrix;
        Boolean boolean;
        Equation* equation;
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
Rval* rval_make_str(char* str);

#endif
