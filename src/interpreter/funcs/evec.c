#include "evec.h"
#include "eval.h"
#include "I.h"
#include "rref.h"
#include "aug.h"

Rval* evec_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: evec(matrix)\n");
        return NULL;
    }

    return evec(args[0]->value.matrix);
}

static Matrix* evec_solve(Matrix* m, double eval) {
    /* can only solve 2x2 matrices */
    Rval *eval_ident, *sub_rref;
    Matrix *evec, *m_eval_ident, *sub, *m_sub_rref;
    /* Av = yv */
    /* (A - yI)v = 0 */

    /* one eigenvalue */
    eval_ident = I(m->nrows);
    m_eval_ident =  matrix_multiply_constant(eval_ident->value.matrix, eval);
    sub = matrix_subtract(m, m_eval_ident);
    sub_rref = rref(sub);
    m_sub_rref = sub_rref->value.matrix;
    evec = matrix_create_dim(m->nrows, 1);

    if(m_sub_rref->values.rows[0]->values.literals[0] == 1) {
        evec->values.rows[0]->values.literals[0] = -1 * m_sub_rref->values.rows[0]->values.literals[1];
    } else {
        evec->values.rows[0]->values.literals[0] = 1;
    }

    evec->values.rows[1]->values.literals[0] = m_sub_rref->values.rows[1]->values.literals[1] == 1 ? 0 : 1;

    rval_destroy(eval_ident);
    rval_destroy(sub_rref);
    matrix_destroy(sub);
    return evec;
}

Rval* evec_eig(Matrix* m, Rval* evals) {
    /* can only solve 2x2 matrices */

    Rval* evec_aug;
    Matrix *evec, *evecs[2];
    /* Av = yv */
    /* (A - yI)v = 0 */

    if(evals->type == RLITERAL) {
        return rval_make_matrix(evec_solve(m, evals->value.literal));
    } else if(evals->type == RLITERAL_ARRAY) {
        /* two eigenvalues */
        evec = matrix_create_dim_uninitialized(m->nrows);
        evec->ncols = 2;

        evecs[1] = evec_solve(m, evals->value.array.literal_array[0]);
        evecs[0] = evec_solve(m, evals->value.array.literal_array[1]);

        return rval_make_matrix_array(evecs, 2);
    } else {
        printf("Error: need eigenvalues to compute eigenvectors\n");
        return NULL;
    }

    return NULL;
}

Rval* evec(Matrix* m) {
    Rval* evals;

    if((evals = eval(m)) == NULL)
        return NULL;

    return evec_eig(m, evals);
}
