#include <stdlib.h>
#include <string.h>
#include "../util.h"

#include "equation.h"

// 100e^(100t)v1 = 13 len
#define PARTLEN 22
#define PLUSLEN 3

Equation* equation_create(double* consts, double* evals, Matrix** evecs, unsigned nparts) {
    Equation* equ = malloc(sizeof(struct Equation));
    equ->consts = consts;
    equ->evals = evals;
    equ->evecs = evecs;
    equ->nparts = nparts;
    return equ;
}

Equation* equation_create_single(double constant, double eval, Matrix* evec) {
    Equation* equ = malloc(sizeof(struct Equation));
    equ->consts = malloc(sizeof(double));
    equ->consts[0] = constant;

    equ->evals = malloc(sizeof(double));
    equ->evals[0] = eval;

    equ->evecs = malloc(sizeof(struct Matrix *));
    equ->evecs[0] = evec;

    equ->nparts = 1;
    return equ;
}

char equation_cmp(Equation* equ1, Equation* equ2) {
    // TODO
}

void equation_print(Equation* equ) {
    unsigned i;
    double const_val;
    char *str, *part;

    if(double_cmp(equ->consts[0], 1) == 0)
        printf("e^(%.6gt)v%d", equ->evals[0], 1);
    else
        printf("%.6g*e^(%.6gt)v%d", equ->consts[0], equ->evals[0], 1);

    for(i = 1; i < equ->nparts; i++) {
        const_val = equ->consts[i];
        if(const_val > 0)
            printf(" + ");
        else {
            const_val *= -1;
            printf(" - ");
        }
        if(double_cmp(equ->consts[i], 1) == 0)
            printf("e^(%.6gt)v%d", equ->evals[i], i+1);
        else
            printf("%.6g*e^(%.6gt)v%d", const_val, equ->evals[i], i+1);
    }
    printf("\n");

    for(i = 0; i < equ->nparts; i++) {
        printf("v%d\n", i+1);
        matrix_print(equ->evecs[i]);
    }
    printf("\n");
}

void equation_destroy(Equation* equ) {
    unsigned i;

    free(equ->consts);
    free(equ->evals);
    for(i = 0; i < equ->nparts; i++)
        matrix_destroy(equ->evecs[i]);
    free(equ->evecs);
    free(equ);
}
