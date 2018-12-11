#include "isstable.h"
#include "eval.h"

Rval* isstable_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: isstable(matrix)\n");
        return NULL;
    }

    return isstable(args[0]->value.matrix);
}

/*
 * Thm 6.5.1
 *
 * If the eigenvalues of a 2x2 matrix C has negative real part,
 * then the origin is an asymptotically stable equilibrium.
 *
 * If one of the eigenvalues of C has positive real part,
 * then the origin is unstable
 */
Rval* isstable(Matrix* m) {
    Rval* evals;
    double* devals;

    if(m->nrows == 2 && m->ncols == 2) {
        evals = eval(m);
        if(evals->type == RLITERAL) {
            if(evals->value.literal < 0)
                return rval_make_boolean(TRUE);
            return rval_make_boolean(FALSE);
        }

        devals = evals->value.array.literal_array;
        if(devals[0] > 0 || devals[1] > 0)
            return rval_make_boolean(FALSE);
        return rval_make_boolean(TRUE);
    }

    printf("Error: can only determine stability of 2x2 matrices\n");
    return NULL;
}
