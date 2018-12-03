#include "det.h"

Rval* det_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: det(matrix)\n");
        return NULL;
    }

    return det(args[0]->value.matrix);
}

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

Rval* det(Matrix* m) {
    if(m->nrows != m->ncols) {
        printf("Cannot calculate determinant of non square matrix.\n");
        return NULL;
    }

    double val = _det(m);
    return rval_make_literal(val);
}
