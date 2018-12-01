#include <stdlib.h>

#include "inverse.h"
#include "I.h"
#include "aug.h"
#include "rref.h"

Rval* inverse_handler(Rval* args, unsigned nargs) {
    if(nargs != 1 || args[0].type != RMATRIX) {
        printf("Usage: inverse(matrix)\n");
        return NULL;
    }

    return inverse(args[0].value.matrix);
}

Rval* inverse(Matrix* m) {
    Matrix *copy;
    Rval *m_aug, *In, *m_aug_rref;

    if(m->ncols != m->nrows) {
        printf("Usage: matrix must be square.\n");
        return NULL;
    }

    copy = matrix_copy(m);
    In = I(m->nrows);

    m_aug = aug(copy, In->value.matrix);
    m_aug_rref = rref(m_aug->value.matrix);
    matrix_slice_after(m_aug_rref->value.matrix, m->ncols);

    matrix_destroy(copy);
    rval_destroy(In);
    rval_destroy(m_aug);
    return m_aug_rref;
}