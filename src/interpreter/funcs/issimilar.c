#include "issimilar.h"
#include "det.h"
#include "trace.h"
#include "../../util.h"
#include "jnform.h"
#include "eval.h"

Rval* issimilar_handler(Rval** args, unsigned nargs) {
    if(nargs != 2 || args[0]->type != RMATRIX || args[1]->type != RMATRIX) {
        printf("Usage: is_similar(matrix, matrix)\n");
        return NULL;
    }

    return issimilar(args[0]->value.matrix, args[1]->value.matrix);
}

/*
 * The nxn matrices B and C are similar if there exists
 * an invertible nxn matrix P such that
 * C = P^-1 * B * P
 *
 * Interesting because if we know the solutions to the system of diff eqs
 * Y* = CY then we also know the solution to the system
 * X* = BX
 */
Rval* issimilar(Matrix* m1, Matrix* m2) {
    /*
     * Suppose that B and C = P^-1 * B * P
     * are similar matrices. If Y(t) is a solution to the differential
     * equations Y* = CY, then X(t) = PY(t) is a solution to the
     * system of differential equations X* = BX
     */
    Rval *det1, *det2, *tr1, *tr2, *jnf1, *jnf2, *evals;

    /* determinants of similar matrices are equal */
    det1 = det(m1);
    det2 = det(m2);
    if(double_cmp(det1->value.literal, det2->value.literal) != 0) {
        rval_destroy(det1);
        rval_destroy(det2);
        return rval_make_boolean(FALSE);
    }

    /* trace of similar matrices are equal */
    tr1 = trace(m1);
    tr2 = trace(m2);
    if(double_cmp(tr1->value.literal, tr2->value.literal) != 0) {
        rval_destroy(det1);
        rval_destroy(det2);
        rval_destroy(tr1);
        rval_destroy(tr2);
        return rval_make_boolean(FALSE);
    }

    /* Similar if matrices have same jordan normal form */
    evals = eval(m1);
    jnf1 = jnform_eval(m1, evals);
    jnf2 = jnform_eval(m2, evals);

    if(matrix_cmp(jnf1->value.matrix, jnf2->value.matrix) == 0) {
        rval_destroy(det1);
        rval_destroy(det2);
        rval_destroy(tr1);
        rval_destroy(tr2);
        rval_destroy(evals);
        rval_destroy(jnf1);
        rval_destroy(jnf2);
        return rval_make_boolean(TRUE);
    }

    rval_destroy(det1);
    rval_destroy(det2);
    rval_destroy(tr1);
    rval_destroy(tr2);
    rval_destroy(evals);
    rval_destroy(jnf1);
    rval_destroy(jnf2);
    return rval_make_boolean(FALSE);
}
