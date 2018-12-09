#include <stdlib.h>
#include <string.h>

#include "equation.h"

// 100e^(100t)v1 = 13 len
#define PARTLEN 22
#define PLUSLEN 3

Equation* equation_create_cplx(double* consts, Cplx** evals, Matrix** evecs, unsigned nparts) {
    unsigned i;

    Equation* equ = malloc(sizeof(struct Equation));
    equ->consts = consts;

    equ->cevals = evals;
    equ->type = CPLX;

    equ->evecs = evecs;
    equ->nparts = nparts;
    return equ;
}

Equation* equation_create_real(double* consts, double* evals, Matrix** evecs, unsigned nparts) {
    Equation* equ = malloc(sizeof(struct Equation));
    equ->consts = consts;
    equ->revals = evals;
    equ->type = REAL;
    equ->evecs = evecs;
    equ->nparts = nparts;
    return equ;
}

char equation_cmp(Equation* equ1, Equation* equ2) {
    unsigned i;
    if(equ1 == equ2)
        return 0;
    if(equ1->nparts != equ2->nparts)
        return 1;
    if(equ1->type != equ2->type)
        return 1;

    for(i = 0; i < equ1->nparts; i++) {
        if(equ1->consts[i] != equ2->consts[i])
            return 1;
        if(equ1->type == REAL)
            if(equ1->revals[i] != equ2->revals[i])
                return 1;
        else
            if(cplx_cmp(equ1->cevals[i], equ2->cevals[i]))
                return 1;
        if(matrix_cmp(equ1->evecs[i], equ2->evecs[i]) != 0)
            return 1;
    }

    return 0;
}

static void equation_print_real(Equation* equ) {
    unsigned i;
    double const_val;
    char *str, *part;

    printf("%.6g*e^(%.6gt)v%d", equ->consts[0], equ->revals[0], 0);
    for(i = 1; i < equ->nparts; i++) {
        const_val = equ->consts[i];
        if(const_val > 0)
            printf(" + ");
        else {
            const_val *= -1;
            printf(" - ");
        }
        printf("%.6g*e^(%.6gt)v%d", const_val, equ->revals[i], i);
    }
    printf("\n");

    for(i = 0; i < equ->nparts; i++) {
        printf("v%d\n", i);
        matrix_print(equ->evecs[i]);
    }
    printf("\n");
}

static void equation_print_cplx(Equation* equ) {
    unsigned i;
    double const_val;
    char *str, *part;

    printf("%.6g*e^(%.6gt)v%d", equ->consts[0], equ->cevals[0], 0);
    for(i = 1; i < equ->nparts; i++) {
        const_val = equ->consts[i];
        if(const_val > 0)
            printf(" + ");
        else {
            const_val *= -1;
            printf(" - ");
        }
        printf("%.6g*e^(%.6gt)v%d", const_val, equ->cevals[i], i);
    }
    printf("\n");

    for(i = 0; i < equ->nparts; i++) {
        printf("v%d\n", i);
        matrix_print(equ->evecs[i]);
    }
    printf("\n");
}

void equation_print(Equation* equ) {
    if(equ->type == REAL)
        equation_print_real(equ);
    else
        equation_print_cplx(equ);
}

void equation_destroy(Equation* equ) {
    unsigned i;

    free(equ->consts);
    if(equ->type == REAL)
        free(equ->revals);
    else {
        for(i = 0; i < equ->nparts; i++)
            cplx_destroy(equ->cevals[i]);
        free(equ->cevals);
    }

    for(i = 0; i < equ->nparts; i++)
        matrix_destroy(equ->evecs[i]);
    free(equ->evecs);
    free(equ);
}
