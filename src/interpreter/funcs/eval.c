#include <math.h>

#include "trace.h"
#include "det.h"
#include "eval.h"
#include "disc.h"
#include "../../util.h"
#include "../cplx.h"

Rval* eval_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: eval(matrix)\n");
        return NULL;
    }

    return eval(args[0]->value.matrix);
}
 
static Rval* eval_2_x_2(Matrix* m) {
    Rval *mtrace, *mdet;
    Cplx *cplxs[2], *cplx;
    double eig, ac4, mtrace_val, m_disc, tau, sigma, factor;
    double eigs[2];

    /* pc = y^2 - tr(c)+det(c)*/
    /* y = (-tc(C) +- sqrt(tr(c) - 4*1*det(c)))/2*/

    mtrace = trace(m);
    mtrace_val = mtrace->value.literal;

    mdet = det(m);
    ac4 = 4 * mdet->value.literal;
    m_disc = discriminant_solve(mtrace_val, mdet->value.literal);

    if(double_cmp(m_disc, 0) == 0) {
        /* real and equal eigenvalue */
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

    /* complex eigenvalues */
    // y^2+1 = (0 +- sqrt(0^2 - 4)) = +-2sqrt(i)/2
    sigma = mtrace->value.literal;
    tau = sqrt(ac4) / 2;
    factor = gcf(sigma, tau);
    sigma /= factor;
    tau /= factor;

    cplx = cplx_create(sigma, tau);
    cplxs[0] = cplx;
    cplx = cplx_create(sigma, -1 * tau);
    cplxs[1] = cplx;

    rval_destroy(mtrace);
    rval_destroy(mdet);
    return rval_make_cplx_array(cplxs, 2);
}
 
Rval* eval(Matrix* m) {
    if(m->nrows == 2 && m->ncols == 2) {
        return eval_2_x_2(m);
    } else {
        // TODO
    }
    printf("Cannot solve for eigenvalues of non 2x2 matrices.\n");
    return NULL;
}
