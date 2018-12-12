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

/* 6.3 Direct Method 
 *
 * a)
 * equation
 * X* = [[y1, 0], [0, y2]]X
 * closed form solution
 * X(t) = [[e^(y1t), 0], [0, e^(y2t)]]
 *
 * b)
 * complex
 *
 * c)
 * equation
 * X* = [[y1, 1], [0, y1]]X
 * closed form solution
 * X(t) = e^(y1t)[[1, t],[0, 1]]X0
 */

static Equation* diffsolve_direct(const Matrix *m, const Matrix* init) {

}

Rval* diffsolve(Matrix *m, Matrix *init) {
    double *consts;
    unsigned i, nmatrices;
    // ic stands for initial condition
    Rval *evals, *evecs, *ic, *ic_rref;
    Matrix **marr, **evec_arr;
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
        nmatrices = 1 + evecs->value.array.length;
        evec_arr = evecs->value.array.matrix_array;
        marr = malloc(nmatrices * sizeof(struct Matrix *));
        for(i = 0; i < nmatrices - 1; i++)
            marr[i] = evec_arr[i];
        marr[i] = init;
    } else {
        printf("Error: diffsolve needs matrix eigenvectors\n");
        rval_destroy(evecs);
        rval_destroy(evals);
        return NULL;
    }

    ic = aug(marr, nmatrices);
    ic_rref = rref_aug(ic->value.matrix);

    consts = malloc((nmatrices - 1) * sizeof(double));
    for(i = 0; i < nmatrices - 1; i++)
        consts[i] = ic_rref->value.matrix->values.rows[i]->values.literals[nmatrices - 1];

    equ = equation_create(consts, evals->value.array.literal_array, evecs->value.array.matrix_array, nmatrices - 1);

    evals->value.array.literal_array = NULL;
    evecs->value.array.literal_array = NULL;
    rval_destroy(evals);
    rval_destroy(evecs);
    free(marr);
    rval_destroy(ic);
    rval_destroy(ic_rref);
    return rval_make_equ(equ);
}
