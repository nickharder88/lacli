#ifndef EQUATION_H
#define EQUATION_H

#include "../matrix.h"
#include "cplx.h"

typedef struct Equation {

    double* consts;

    enum {
        REAL, CPLX
    } type;

    union {
        double* revals;
        Cplx** cevals;
    };

    Matrix** evecs;

    unsigned nparts;
} Equation;

Equation* equation_create_cplx(double* consts, Cplx** evals, Matrix** evecs, unsigned nparts);
Equation* equation_create_real(double* consts, double* evals, Matrix** evecs, unsigned nparts);
char equation_cmp(Equation* equ1, Equation* equ2);
void equation_print(Equation* equ);
void equation_destroy(Equation* equ);

#endif
