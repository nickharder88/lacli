#include <stdlib.h>

#include "linind.h"
#include "rref.h"
#include "I.h"
#include "aug.h"

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
    Matrix *m = NULL, *col, *col_copy;
    Rval *m_rref, *In, *m_aug;

    for(i = 0; i < ncols; i++) {
        if((col = col_vectors[i])->ncols != 1) {
            printf("Error: can only check linear dependence on column vectors\n");
            matrix_destroy(m);
            return NULL;
        }

        if(m == NULL) {
            m = matrix_copy(col);
        } else {
            col_copy = matrix_copy(col);
            if((m_aug = aug(m, col_copy)) == NULL) {
                printf("Error: could not augment matrices while checking linear independence\n");
                matrix_destroy(col_copy);
                matrix_destroy(m);
                return NULL;
            }

            matrix_destroy(m);
            matrix_destroy(col_copy);

            m = m_aug->value.matrix;

            m_aug->type = RNIL;
            rval_destroy(m_aug);
        }
    }

    m_rref = rref(m);
    In = I(ncols);
    is_dep = matrix_cmp(m_rref->value.matrix, In->value.matrix) != 0;

    matrix_destroy(m);
    rval_destroy(In);
    rval_destroy(m_rref);
    return rval_make_boolean(is_dep ? FALSE : TRUE);
}
