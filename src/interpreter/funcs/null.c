#include "null.h"
#include "rref.h"
#include "rank.h"

/*
 * null space is the set of solutions to the homogeneous
 * system of linear equations Ax=0
 */

Rval* null_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: null(matrix)\n");
        return NULL;
    }

    return null(args[0]->value.matrix);
}

Rval* null(Matrix* m) {
    unsigned mrank;
    Matrix* nspace;
    Rval* val;

    val = rref(m);
    mrank = rank_rref(val->value.matrix);

    /* the null space of A is the span of n-l vectors */


    return rval_make_matrix(nspace);
}
