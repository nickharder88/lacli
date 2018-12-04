#include <stdlib.h>

#include "linind.h"
#include "rref.h"
#include "I.h"
#include "aug.h"
#include "rank.h"

Rval* linind_handler(Rval** args, unsigned nargs) {
    unsigned i;
    Matrix **col_vectors;
    Rval* val;

    if(nargs == 0) {
        printf("Usage: linind(matrix, ...)\n");
        return NULL;
    }

    col_vectors = malloc(nargs * sizeof(struct Matrix *));

    for(i = 0; i < nargs; i++) {
        if(args[i]->type != RMATRIX) {
            printf("Usage: lindind(matrix, ...)\n");
            free(col_vectors);
            return NULL;
        }

        col_vectors[i] = args[i]->value.matrix;
    }

    val = linind(col_vectors, nargs);
    free(col_vectors);
    return val;
}

Rval* linind(Matrix** col_vectors, unsigned ncols) {
    char is_dep;
    unsigned char col_checked = 0;
    unsigned i;
    Rval *m_rref, *In, *m_aug, *m_rank;
    Matrix* m_cmp;

    for(i = 0; i < ncols; i++) {
        if(col_vectors[i]->ncols != 1) {
            printf("Error: can only check linear dependence on column vectors\n");
            return NULL;
        }
    }

    m_aug = aug(col_vectors, ncols);
    m_rref = rref(m_aug->value.matrix);
    m_rank = rank(m_rref->value.matrix);
    m_cmp = matrix_slice_below(m_rref->value.matrix, m_rank->value.literal);
    In = I(ncols);

    is_dep = matrix_cmp(m_cmp, In->value.matrix) != 0;

    rval_destroy(m_aug);
    rval_destroy(In);
    rval_destroy(m_rref);
    matrix_destroy(m_cmp);
    return rval_make_boolean(is_dep ? FALSE : TRUE);
}
