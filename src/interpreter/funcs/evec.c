#include "evec.h"
#include "eval.h"
#include "I.h"
#include "rref.h"
#include "aug.h"
#include "../../util.h"

Rval* evec_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: evec(matrix)\n");
        return NULL;
    }

    return evec(args[0]->value.matrix);
}

static Matrix* evec_solve_generalized(Matrix* m, Matrix* v1, double eval) {
    /* can only solve 2x2 matrices */
    double *r1lits, *r2lits, w1, w2;
    Rval *eval_ident, *gen_rref, *gen_aug;
    Matrix *evec, *m_eval_ident, *sub, *m_gen_rref, *gen_matrices[2];
    /* Av = yv */
    /* (A - yI)v = 0 */

    /* one eigenvalue */
    eval_ident = I(m->nrows);
    m_eval_ident =  matrix_multiply_constant(eval_ident->value.matrix, eval);
    sub = matrix_subtract(m, m_eval_ident);

    gen_matrices[0] = sub;
    gen_matrices[1] = v1;
    gen_aug = aug(gen_matrices, 2);

    gen_rref = rref_aug(gen_aug->value.matrix);
    m_gen_rref = gen_rref->value.matrix;

    evec = matrix_create_dim(m->nrows, 1);
    /* analyze generalized matrix to create eigenvector */
    /* [1, -1, -1] w1 = 1, w2 = 2 */
    r1lits = m_gen_rref->values.rows[0]->values.literals;
    r2lits = m_gen_rref->values.rows[1]->values.literals;

    if(double_cmp(r1lits[0], 1) == 0) {
        /* w1 has a defined value */
        if(double_cmp(r1lits[1], 0) == 0) {
            /* [[1,0,x], [0,x,x]] */ 
            evec->values.rows[0]->values.literals[0] = r1lits[2];

            if(double_cmp(r2lits[1], 1) == 0) {
                /* w2 has a defined value 
                 * [[1,0,x], [0,1,x]]
                 */
                evec->values.rows[1]->values.literals[0] = r2lits[2];
            } else {
                /* w2 can be any number
                 * [[1,0,x], [0,0,0]]
                 */
                evec->values.rows[1]->values.literals[0] = 1;
            }
        } else {
            /* w1 and w2 are related
             * [[1,y,x], [0,0,0]]
             * w1 = 1
             * w2 = (x-1) / y
             */
            evec->values.rows[0]->values.literals[0] = 1;
            w2 = (r1lits[2] - 1) / r1lits[1];
            evec->values.rows[1]->values.literals[0] = w2;
        }
    } else if(double_cmp(r1lits[1], 1) == 0) {
        /* w1 can be any value. w2 has a defined value 
         * [[0,1,x],[0,0,0]]
         */
        evec->values.rows[0]->values.literals[0] = 1;
        evec->values.rows[1]->values.literals[0] = r1lits[2];
    } else {
        /* w1 and w2 can be any value */
        evec->values.rows[0]->values.literals[0] = 1;
        evec->values.rows[1]->values.literals[0] = 1;
    }

    rval_destroy(eval_ident);
    matrix_destroy(m_eval_ident);
    matrix_destroy(sub);

    rval_destroy(gen_aug);
    rval_destroy(gen_rref);
    return evec;
}

static Matrix* evec_solve(Matrix* m, double eval) {
    /* can only solve 2x2 matrices */
    Rval *eval_ident, *sub_rref;
    Matrix *evec, *m_eval_ident, *sub, *m_sub_rref;
    double *r1lit, *r2lit;
    /* Av = yv */
    /* (A - yI)v = 0 */

    /* one eigenvalue */
    eval_ident = I(m->nrows);
    m_eval_ident =  matrix_multiply_constant(eval_ident->value.matrix, eval);
    sub = matrix_subtract(m, m_eval_ident);
    sub_rref = rref(sub);

    m_sub_rref = sub_rref->value.matrix;
    r1lit = m_sub_rref->values.rows[0]->values.literals;
    r2lit = m_sub_rref->values.rows[1]->values.literals;

    evec = matrix_create_dim(m->nrows, 1);

    /* set both to be 1 ~ aka they can be any values */
    evec->values.rows[0]->values.literals[0] = 1;
    evec->values.rows[1]->values.literals[0] = 1;

    if(double_cmp(r1lit[0], 1) == 0) {
        /* x1 has a defined value */
        if(double_cmp(r1lit[1], 0) == 0) {
            /* x1 is equal to 0 
             * [[1,0],[0,x]]
             */
            evec->values.rows[0]->values.literals[0] = 0;
        } else {
            /* set x1 equal to 1 
             * [[1,x],[0,0]]
             */
            evec->values.rows[1]->values.literals[0] = round_if_insignificant(-1 * r1lit[1]);

            /* [[1,x],[0,1]] not allowed */
            if(double_cmp(r2lit[1], 0) != 0) {
                printf("Error: cannot create relation between x values when determining eigenvectors\n");
                rval_destroy(eval_ident);
                matrix_destroy(m_eval_ident);
                matrix_destroy(sub);
                rval_destroy(sub_rref);
                matrix_destroy(evec);
                return NULL;
            }
        }
    }

    if(double_cmp(r2lit[1], 1) == 0) {
        /* x2 is 0*/
        evec->values.rows[1]->values.literals[0] = 0;
    }

    rval_destroy(eval_ident);
    matrix_destroy(m_eval_ident);
    matrix_destroy(sub);
    rval_destroy(sub_rref);
    return evec;
}

Rval* evec_eig(Matrix* m, Rval* evals) {
    /* can only solve 2x2 matrices */

    Rval* evec_aug;
    Matrix *evecs[2];
    /* Av = yv */
    /* (A - yI)v = 0 */

    if(evals->type == RLITERAL) {
        evecs[0] = evec_solve(m, evals->value.literal);
        evecs[1] = evec_solve_generalized(m ,evecs[0], evals->value.literal);
        return rval_make_matrix_array(evecs, 2);
    } else if(evals->type == RLITERAL_ARRAY) {
        /* two eigenvalues */
        evecs[0] = evec_solve(m, evals->value.array.literal_array[0]);
        evecs[1] = evec_solve(m, evals->value.array.literal_array[1]);

        return rval_make_matrix_array(evecs, 2);
    } else {
        printf("Error: need eigenvalues to compute eigenvectors\n");
        return NULL;
    }

    return NULL;
}

Rval* evec(Matrix* m) {
    Rval *evals, *evecs;

    if((evals = eval(m)) == NULL)
        return NULL;

    evecs = evec_eig(m, evals);
    rval_destroy(evals);
    return evecs;
}
