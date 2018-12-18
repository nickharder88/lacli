#ifndef EQUATION_H
#define EQUATION_H

#include "../matrix.h"

typedef struct Equation {
    double* consts;
    double* evals;
    Matrix** evecs;

    unsigned nparts;
} Equation;

Equation* equation_create(double* consts, double* evals, Matrix** evecs, unsigned nparts);
Equation* equation_create_single(double constant, double eval, Matrix* evec);
char equation_cmp(Equation* equ1, Equation* equ2);
void equation_print(Equation* equ);
void equation_destroy(Equation* equ);

#endif
