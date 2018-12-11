#include "linearmap.h"
#include "transpose.h"
#include "aug.h"
#include "inverse.h"

Rval* linearmap_handler(Rval** args, unsigned nargs) {
    unsigned nrow_vectors, i;
    Matrix **vrows, **wrows;

    if(nargs == 0 || args[0]->type != RLITERAL) {
        printf("Usage: linearmap(nrow_vectors, v1,...,vn, w1,...,wn)\n");
        return NULL;
    }

    nrow_vectors = (unsigned) args[0]->value.literal;
    if(nargs - 1 != nrow_vectors) {
        printf("Usage: linearmap(nrow_vectors, v1,...,vn, w1,...,wn)\n");
        return NULL;
    }

    for(i = 1; i < nargs; i++) {
        if(args[i]->type != RMATRIX || args[i]->value.matrix->nrows != 1) {
            printf("Usage: linearmap(nrow_vectors, v1,...,vn, w1,...,wn)\n"
                    "Where v1,...,vn and w1,...,wn are row vectors\n");
            return NULL;
        }

        if(i < nrow_vectors)
            vrows[i-1] = args[i]->value.matrix;
        else
            wrows[i-1] = args[i]->value.matrix;
    }

    return linearmap(vrows, wrows, nrow_vectors);
}

/*
 * Theorem 8.1.2
 * Let V and W be vector spaces. Let {v1,...,vn} be a basis for V and
 * let {w1,...,wn} be n vectors in W. Then there exists a unique linear map
 * L : V->W such that L(vi) = wi
 */
Rval* linearmap(Matrix** vrows, Matrix** wrows, unsigned nrow_vectors) {
    unsigned i;
    Rval *trans, *vaug, *waug, *vinv;
    Matrix *vtranspi[nrow_vectors], *wtranspi[nrow_vectors], *map;

    for(i = 0; i < nrow_vectors; i++) {
        trans = transpose(vrows[i]);
        vtranspi[i] = trans->value.matrix;

        trans->value.matrix = NULL;
        rval_destroy(trans);
    }

    vaug = aug(vtranspi, nrow_vectors);
    if((vinv = inverse(vaug->value.matrix)) == NULL) {
        printf("Error: row vectors do not form a basis\n");
        rval_destroy(vaug);
        for(i = 0; i < nrow_vectors; i++)
            matrix_destroy(vtranspi[i]);
        return NULL;
    }

    for(i = 0; i < nrow_vectors; i++) {
        trans = transpose(wrows[i]);
        wtranspi[i] = trans->value.matrix;

        trans->value.matrix = NULL;
        rval_destroy(trans);
    }

    waug = aug(wtranspi, nrow_vectors);
    map = matrix_multiply(waug->value.matrix, vinv->value.matrix);

    rval_destroy(vaug);
    rval_destroy(vinv);
    rval_destroy(vinv);
    for(i = 0; i < nrow_vectors; i++) {
        matrix_destroy(vtranspi[i]);
        matrix_destroy(wtranspi[i]);
    }

    return rval_make_matrix(map);
}
