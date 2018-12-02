#include <math.h>

#include "trace.h"
#include "det.h"
#include "eval.h"

Rval* eval_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: eval(matrix)\n");
        return NULL;
    }

    return eval(args[0]->value.matrix);
}

Rval* eval(Matrix* m) {
    Rval *mtrace, *mdet;
    double eig, ac4, mtrace_val;
    double eigs[2];

    /* pc = y^2 - tr(c)+det(c)*/
    /* y = (-tc(C) +- sqrt(tr(c) - 4*1*det(c)))/2*/

    mtrace = trace(m);
    mtrace_val = mtrace->value.literal;

    mdet = det(m);
    ac4 = 4 * mdet->value.literal;

    if(ac4 > mtrace_val) {
        printf("Complex eigenvalues not available.\n");
        rval_destroy(mtrace);
        rval_destroy(mdet);
        return NULL;
    }

    eigs[0] = (mtrace->value.literal + sqrt(mtrace_val * mtrace_val - ac4)) / 2;
    eigs[1] = (mtrace->value.literal - sqrt(mtrace_val * mtrace_val - ac4)) / 2;

    rval_destroy(mtrace);
    rval_destroy(mdet);
    return rval_make_literal_array(eigs, 2);
}
