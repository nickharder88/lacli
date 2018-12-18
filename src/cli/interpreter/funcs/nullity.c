#include "nullity.h"
#include "rank.h"

Rval* nullity_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: nullity(matrix)\n");
        return NULL;
    }

    return nullity(args[0]->value.matrix);
}

Rval* nullity(Matrix* m) {
    Rval *mrank;
    unsigned mnullity;

    /* 
     * The nullity of M(mxn) is the dimension of the null space of A 
     *
     * nullity(m) + rank(m) = n
     */
    mrank = rank(m);
    mnullity = m->ncols - mrank->value.literal;

    rval_destroy(mrank);
    return rval_make_literal(mnullity);
}
