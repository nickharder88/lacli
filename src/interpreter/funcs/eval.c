#include <math.h>

#include "trace.h"
#include "det.h"
#include "eval.h"
#include "disc.h"
#include "../../util.h"

Rval* eval_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: eval(matrix)\n");
        return NULL;
    }

    return eval(args[0]->value.matrix);
}
 
static Rval* eval_2_x_2(Matrix* m) {
    Rval *mtrace, *mdet;
    double eig, ac4, mtrace_val, m_disc;
    double eigs[2];

    /* pc = y^2 - tr(c)+det(c)*/
    /* y = (-tc(C) +- sqrt(tr(c) - 4*1*det(c)))/2*/

    mtrace = trace(m);
    mtrace_val = mtrace->value.literal;

    mdet = det(m);
    ac4 = 4 * mdet->value.literal;
    m_disc = discriminant_solve(mtrace_val, mdet->value.literal);

    if(double_cmp(m_disc, 0) == 0) {
        /* real and equal eigenvalues */
        /* 
         * 6.2 
         * There are two types of 2x2 matrices that have real and equal eigenvalues
         * Those that are scalar multiples of the identity and those that are not.
         */
        eigs[0] = (mtrace->value.literal + sqrt(mtrace_val * mtrace_val - ac4)) / 2;
        rval_destroy(mtrace);
        rval_destroy(mdet);
        return rval_make_literal(eigs[0]);
    } else if(m_disc > 0) {
        /* real and distinct eigenvalues */
        eigs[0] = (mtrace->value.literal + sqrt(mtrace_val * mtrace_val - ac4)) / 2;
        eigs[1] = (mtrace->value.literal - sqrt(mtrace_val * mtrace_val - ac4)) / 2;

        rval_destroy(mtrace);
        rval_destroy(mdet);
        return rval_make_literal_array(eigs, 2);
    }

    /* complex eigenvalues*/
    printf("Complex eigenvalues unsolvable\n");
    rval_destroy(mtrace);
    rval_destroy(mdet);
    return NULL;
}

/*
 * Definition 7.2.1
 *
 * Let A be an nxn matrix. The characteristic polynomial of A is
 * p(lamda) = det(A - lamda*In)
 *
 * Thm 7.2.3
 * Let A be an nxn matrix. The p is a polynomial of degree n of the form
 * p(lamda) = (-1)^n*lamda^n + b(n-1)*lamda^(n-1)+...+b(0)
 * for some real constants b0...b(n-1)
 */
static Rval* eval_n_x_n(Matrix *m) {
    return NULL;
}
 
Rval* eval(Matrix* m) {
    if(m->nrows == 2 && m->ncols == 2) {
        return eval_2_x_2(m);
    } else {
        return eval_n_x_n(m);
    }
    printf("Cannot solve for eigenvalues of non 2x2 matrices.\n");
    return NULL;
}
