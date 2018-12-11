#include "det.h"
#include "rref.h"

Rval* det_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: det(matrix)\n");
        return NULL;
    }

    return det(args[0]->value.matrix);
}

Rval* detrr_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: det(matrix)\n");
        return NULL;
    }

    return detrr(args[0]->value.matrix);
}

/*
 * A determinant function of a square nxn matrix A is a real number D(A) that
 * satisifes three properties
 *
 * a) if A = (aij) is a lower triangular, then D(A) is the product
 * of the diagonal entries (trace)
 *
 * b) D(A^t) = D(A)
 *
 * c) Let B be an nxn matrix. Then
 *  D(AB) = D(A)D(B)
 *
 *
 *  Thm 7.1.7
 *  An nxn matrix A is invertible iff det(A) != 0. If A^-1 exists,
 *  det(A^-1) = 1/(detA)
 *
 *  Corollary 7.1.8
 *  If the rows of an nxn matrix A are linearly dependent,
 *  then det(A) = 0
 *
 *  Lemma 7.1.9
 *  Let A be an NxN matrix of the form A = [[B,0],[C,D]]
 *  then det(A) = det(B)det(D)
 */
static double _det(Matrix* m) {
    unsigned i;
    char sign = -1;
    double val = 0;
    Matrix *row, *sub_matrix;

    if(m->nrows == 1)
        return m->values.literals[0];

    for(i = 0; i < m->ncols; i++) {
        sign *= -1;
        row = m->values.rows[0];
        sub_matrix = matrix_copy_remove_row_col(m, 0, i);
        val += sign * row->values.literals[i] * _det(sub_matrix);
        matrix_destroy(sub_matrix);
    }
}

Rval* det(Matrix *m) {
    double val;
    if(m->nrows != m->ncols) {
        printf("Matrix must be square to calculate determinant.\n");
        return NULL;
    }

    val = _det(m);
    return rval_make_literal(val);
}

/* Compute determinant by row reducing*/
Rval* detrr(Matrix *m) {
    if(m->nrows != m->ncols) {
        printf("Matrix must be square to calculate determinant.\n");
        return NULL;
    }

    return rref_det(m);
}
