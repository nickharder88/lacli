#include "stableorbit.h"
#include "eval.h"
#include "evec.h"
#include "../equation.h"

Rval* stableorbit_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: stableorbit(matrix)\n");
        return NULL;
    }
}

/*
 * stable orbit/manifold consists of those trajectories that limit
 * on the origin in forward time.
 *
 * unstable orbit/manifold consist of those trajectories that limit
 * on the origin in backward time
 */
Rval* stableorbit(Matrix *m) {
    Rval *evals, *evec ;
    double *devals, deval;
    Equation* equ;

    if(m->nrows != 2 || m->ncols != 2) {
        printf("Error: stableorbit expects a 2x2 matrix\n");
        return NULL;
    }

    evals = eval(m);
    if(evals->type != RLITERAL_ARRAY) {
        printf("Error: stableorbit operates on a matrix with 2 eigenvalues\n");
        rval_destroy(evals);
        return NULL;
    }

    evec = evec_eig(m, evals);
    devals = evals->value.array.literal_array;

    if(devals[0] < 0 && devals[1] > 0)
        equ = equation_create_single(1, devals[0], evec->value.array.matrix_array[0]);
    else if(devals[1] < 0 && devals[0] > 0)
        equ = equation_create_single(1, devals[1], evec->value.array.matrix_array[1]);
    else {
        printf("Error: stableorbit operates on a matrix with the origin as a saddle\n");
        rval_destroy(evals);
        rval_destroy(evec);
        return NULL;
    }

    /* dont free matrix array */
    evec->value.array.matrix_array = NULL;
    rval_destroy(evals);
    rval_destroy(evec);
    return rval_make_equ(equ);
}
