#include <stdlib.h>

#include "inverse.h"
#include "I.h"
#include "aug.h"
#include "rref.h"
#include "det.h"

Rval* inverse_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: inverse(matrix)\n");
        return NULL;
    }

    return inverse(args[0]->value.matrix);
}

Rval* inverse(Matrix* m) {
    Matrix *copy, *marr[2];
    Rval *m_aug, *In, *m_aug_rref, *m_det;

    if(m->ncols != m->nrows) {
        printf("Usage: matrix must be square.\n");
        return NULL;
    }

    if((m_det = det(m)) == NULL || m_det->value.literal == 0) {
        printf("Cannot inverse a matrix with determinant equal to 0\n");
        return NULL;
    }

    copy = matrix_copy(m);
    In = I(m->nrows);

    marr[0] = copy;
    marr[1] = In->value.matrix;
    m_aug = aug(marr, 2);
    m_aug_rref = rref(m_aug->value.matrix);
    matrix_slice_after(m_aug_rref->value.matrix, m->ncols);

    matrix_destroy(copy);
    rval_destroy(m_det);
    rval_destroy(In);
    rval_destroy(m_aug);
    return m_aug_rref;
}
