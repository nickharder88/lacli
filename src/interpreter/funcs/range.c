#include "range.h"
#include "basis.h"

Rval* range_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: range(matrix)\n");
        return NULL;
    }

    return range(args[0]->value.matrix);
}

/*
 * Lemma 8.2.2
 *
 * Let L : V -> W be a linear map between vector spaces. Then the
 * null space of L is a subspace of V and the range of L
 * is a subspace of W
 *
 * range(L) = {L(v) in W : v in V } is a subset of W
 * 
 * Let w1 and w2 be in the range of L. Then, by definition, there exist
 * v1 and v2 in V such that L(vj) = wj. It follows that
 *
 *     L(v1 + v2) = L(v1) + L(v2) = w1 + w2
 * Therefore, w1+w2 is in the range of L. Similaryl,
 *     L(c*v1) = cL(v1) = cw1
 * So the range of L is closed under addition and scalar multiplication
 * and is a subspace of W
 *
 * Suppose A is an mxn matrix and La : Rn->Rm is the associated linear
 * map. Then the range of La is the column space of A.
 *
 * To verify this, write A = (A1|...|An) where Aj is the jth column of A
 * and let v = transpose(v1,...,vn). Then La(v) is the linear combination
 * of columns of A
 *      La(v) = Av = v1A1 + ... + vnAn
 */
Rval* range(Matrix *m) {
    unsigned row_i, col_i;
    Matrix *cols[m->ncols], *col;

    for(col_i = 0; col_i < m->ncols; col_i++) {
        cols[col_i] = col = matrix_create_dim(m->nrows, 1);
        for(row_i = 0; row_i < m->nrows; row_i++) {
            col->values.rows[row_i]->values.literals[0] = m->values.rows[row_i]->values.literals[col_i];
        }
    }

    return basis(cols, m->ncols);
}
