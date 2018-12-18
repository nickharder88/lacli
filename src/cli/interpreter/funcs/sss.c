#include "sss.h"
#include "eval.h"

Rval* sss_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: sss(matrix)\n");
        return NULL;
    }

    return sss(args[0]->value.matrix);
}

Rval* sss(Matrix* m) {
    Rval* evals;
    double* devals;

    if(m->nrows != 2 || m->ncols != 2) {
        printf("Error: sss requires a 2x2 matrix\n");
        return NULL;
    }

    if((evals = eval(m)) == NULL)
        return NULL;

    if(evals->type == RLITERAL) {
        if(evals->value.literal < 0)
            return rval_make_str("sink");
        return rval_make_str("source");
    }

    devals = evals->value.array.literal_array;

    /*
     * If both evals of m are < 0 then sink
     * If both evals of m are > 0 then source
     * If eval1 < 0 and eval2 > 0 OR eval1 > 0 and eval2 < 0 then saddle
     */
    if(devals[0] > 0) {
        if(devals[1] > 0)
            return rval_make_str("source");
    } else if(devals[1] < 0)
        return rval_make_str("sink");

    return rval_make_str("saddle");
}
