#include <ctype.h>

#include "rref.h"


/* expects a string (arg1, arg2,..., argn) */
Rval* rref_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: rref(matrix)\n");
        return NULL;
    }

    return rref(args[0]->value.matrix);
}

static unsigned left_most_nz_row(Matrix* m, unsigned cur_row, unsigned* left_most_col) {
    unsigned row_i, col_i, left_most_row;
    Matrix* row;

    *left_most_col = m->ncols;
    left_most_row = cur_row;
    for(row_i = cur_row; row_i < m->nrows; row_i++) {
        row = m->values.rows[row_i];
        for(col_i = 0; col_i < m->ncols; col_i++)
            if(row->values.literals[col_i]) { /* first non zero in row */
                if(*left_most_col > col_i) {
                    *left_most_col = col_i;
                    left_most_row = row_i;
                }
                break;
            }
    }

    return left_most_row;
}

static void swap(Matrix* m, unsigned i, unsigned j) {
    Matrix *tmp = m->values.rows[i];
    m->values.rows[i] = m->values.rows[j];
    m->values.rows[j] = tmp;
}

static void make_pivot(Matrix* row, unsigned cur_col) {
    unsigned i;
    for(i = cur_col + 1; i < row->ncols; i++)
        row->values.literals[i] /= row->values.literals[cur_col];
    row->values.literals[cur_col] = 1;
}

unsigned get_pivot(Matrix* row) {
    unsigned col;
    for(col = 0; col < row->ncols; col++) {
        if(row->values.literals[col] > 0)
            return col;
    }
}

static void reduce_below(Matrix* m, unsigned row_i) {
    unsigned col_i, below_row_i;
    double scalar;
    unsigned pivot;
    Matrix *row = NULL, *below_row = NULL;

    row = m->values.rows[row_i];
    for(below_row_i = row_i+1; below_row_i < m->nrows; below_row_i++) {
        below_row = m->values.rows[below_row_i];
        pivot = get_pivot(below_row);
        scalar = below_row->values.literals[pivot];
        for(col_i = pivot; col_i < m->ncols; col_i++)
            below_row->values.literals[col_i] -= scalar * row->values.literals[col_i];
    }
}

static void reduce_above(Matrix* m, unsigned row_i) {
    unsigned above_row_i, col_i;
    unsigned pivot;
    double scalar;
    Matrix *row = NULL, *above_row = NULL;

    /* for each row */
    /* get pivot pos */
    /* for each row above this row */
    /* use scalar mult to reduce each column */
    row = m->values.rows[row_i];
    pivot = get_pivot(row);
    for(above_row_i = 0; above_row_i < row_i; above_row_i++) {
        above_row = m->values.rows[above_row_i];

        scalar = above_row->values.literals[pivot];
        for(col_i = pivot; col_i < m->ncols; col_i++)
            above_row->values.literals[col_i] -= scalar * row->values.literals[col_i];
    }
}

static Matrix* ref(Matrix* m) {
    /* cur_row -> index of row we are interested in swapping 
     * mov_row -> index or row we want to swap into cur_row
     *
     * if cur_col == copy->ncols, then its a zero row
     */
    unsigned cur_row, cur_col, move_row;
    Matrix *row, *copy = matrix_copy(m);

    // TODO what if its only one row?

    for(cur_row = 0;
            (move_row = left_most_nz_row(copy, cur_row, &cur_col)) < copy->nrows;
            cur_row++) {
        /* place left most row in next highest position */
        if(cur_row != move_row)
            swap(m, cur_row, move_row);

        /* don't operate on a zero row */
        if(cur_col == copy->ncols)
            continue;

        row = copy->values.rows[cur_row];
        /* make first element a pivot */
        make_pivot(row, cur_col);

        /* reduce all the rows below to 0 */
        if(cur_col < copy->ncols)
            reduce_below(copy, cur_row);
    }

    return copy;
}

/*
 * Creates a copy of matrix m and puts it in rref
 * if identifier is NULL, then it replaces m with the copy
 */
Rval* rref(Matrix* m) {
    unsigned row_i, pivot;
    Matrix *row, *copy = ref(m);
    for(row_i = 0; row_i < copy->nrows; row_i++) {
        row = copy->values.rows[row_i];
        pivot = get_pivot(row);
        if(pivot < copy->ncols) /* is this a non zero row */
            reduce_above(copy, row_i);
    }
    return rval_make_matrix(copy);
}
