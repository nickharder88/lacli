#include "jnform.h"
#include "eval.h"
#include "evec.h"
#include "aug.h"
#include "inverse.h"

Rval* jnform_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: jnform(matrix)\n");
        return NULL;
    }

    return jnform(args[0]->value.matrix);
}

/* B = P^(-1)*M*P */
Rval* jnform(Matrix* m) {
    Rval *evals, *jnf;
    evals = eval(m);
    return jnform_eval(m, evals);
}

Rval* jnform_eval(Matrix *m, Rval* evals) {
    Rval *evecs, *p, *pinv;
    Matrix *pinvm;

    if(m->nrows != 2 || m->ncols != 2) {
        printf("Error: jordan normal form analyzes 2x2 matrices\n");
        return NULL;
    }

    evecs = evec_eig(m, evals);
    if(evecs->type == RMATRIX) {
        /* TODO */
    } else if(evecs->type == RMATRIX_ARRAY) {
        p = aug(evecs->value.array.matrix_array, 2);
        pinv = inverse(p->value.matrix);
        pinvm = matrix_multiply(pinv->value.matrix, m);
        m = matrix_multiply(pinvm, p->value.matrix);
        return rval_make_matrix(m);
    }

    return NULL;
}
