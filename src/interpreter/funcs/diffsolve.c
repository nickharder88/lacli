#include <stdlib.h>
#include <string.h>

#include "diffsolve.h"
#include "eval.h"
#include "evec.h"
#include "aug.h"
#include "rref.h"
#include "../equation.h"

Rval* diffsolve_handler(Rval** args, unsigned nargs) {
    if(nargs != 2 || args[0]->type != RMATRIX || args[1]->type != RMATRIX) {
        printf("Usage: diffsolve(matrix, initial_value)\n");
        return NULL;
    }

    if(args[1]->value.matrix->ncols != 1) {
        printf("Error: initial value must be a column matrix\n");
        return NULL;
    }

    return diffsolve(args[0]->value.matrix, args[1]->value.matrix);
}

Rval* diffsolve(Matrix *m, Matrix *init) {
    double *consts;
    unsigned i, nmatrices;
    // ic stands for initial condition
    Rval *evals, *evecs, *ic, *ic_rref;
    Matrix **marr, **evec_arr;
    Cplx **carr, **cevec_arr;
    Equation *equ;

    // eigenvalues
    evals = eval(m);

    // eigenvectors
    evecs = evec_eig(m, evals);

    // alpha beta
    if(evecs->type == RMATRIX) {
        nmatrices = 2;
        marr = malloc(nmatrices * sizeof(struct Matrix *));
        marr[0] = evecs->value.matrix;
        marr[1] = init;
    } else if(evecs->type == RMATRIX_ARRAY){
        nmatrices = 1 + evecs->value.array->length;
        evec_arr = evecs->value.array->matrices;
        marr = malloc(nmatrices * sizeof(struct Matrix *));
        for(i = 0; i < nmatrices - 1; i++)
            marr[i] = evec_arr[i];
        marr[i] = init;
    } else if(evecs->type == RCPLX_ARRAY) {
        carr = malloc(nmatrices * sizeof(struct Cplx *));
        nmatrices = 1 + evecs->value.array->length;
        cevec_arr = evecs->value.array->cplxs;
        marr = malloc(nmatrices * sizeof(struct Matrix *));
        for(i = 0; i < nmatrices - 1; i++)
            marr[i] = evec_arr[i];
        marr[i] = init;
    }

    ic = aug(marr, nmatrices);
    ic_rref = rref_aug(ic->value.matrix);

    consts = malloc((nmatrices - 1) * sizeof(double));
    for(i = 0; i < nmatrices - 1; i++)
        consts[i] = ic_rref->value.matrix->values.rows[i]->values.literals[nmatrices - 1];

    equ = equation_create_real(consts, evals->value.array->literals, evecs->value.array->matrices, nmatrices - 1);

    evals->value.array->literals = NULL;
    evecs->value.array->literals = NULL;
    rval_destroy(evals);
    rval_destroy(evecs);
    rval_destroy(ic);
    rval_destroy(ic_rref);
    return rval_make_equ(equ);
}
