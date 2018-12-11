#include "classifyjnf.h"
#include "../../util.h"
#include "jnform.h"

Rval* classifyjnf_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: jnf(matrix)\n");
        return NULL;
    }

    return classifyjnf(args[0]->value.matrix);
}

/*
 * Classification of Jordan normal form 2x2 matrices
 *
 * Let C and P = [v1, v2] be 2x2 matrices where v1 and v2 are specified below
 * e1 = [[1], [0]], e2 = [[0], [1]]
 *
 * a)
 * suppose C has two linearly independent real eigenvectors v1 and v2 with real
 * eigenvalues y1 and y2, then
 *
 * P^-1CP = [[y1,0], [0,y2]]
 *
 * b)
 * complex
 *
 * c)
 * Suppose C has exactly one linearly independent real eigenvector v1
 * with real eigenvalue y1. then
 * P-1CP = [[y1, 1], [0, y1]]
 *
 * where v2 is a generalized eigenvector of C that satisfies
 * (C- y1*I2)v2 = v1
 */

Rval* classifyjnf(Matrix *m) {
    Equation* equ;
    Matrix* jnf;
    Rval *rjnf;
    double a12;

    if(m->nrows != 2 || m->ncols != 2) {
        printf("Error: can only classify 2x2 matrices\n");
        return NULL;
    }

    rjnf = jnform(m);
    jnf = rjnf->value.matrix;

    if(double_cmp(jnf->values.rows[1]->values.literals[0], 0) == 0) {
        a12 = jnf->values.rows[0]->values.literals[1];
        if(double_cmp(a12, 0) == 0) {
            /* TODO case a */
        } else if(double_cmp(a12, 1) == 0) {
            /* TODO case c */
        } else {
            printf("Cannot classify with jordan normal form\n");
            matrix_destroy(jnf);
            return NULL;
        }
    } else {
        printf("Cannot classify with jordan normal form\n");
        matrix_destroy(jnf);
        return NULL;
    }

    /* dont free matrices */
    matrix_destroy(jnf);
    return NULL;
}
