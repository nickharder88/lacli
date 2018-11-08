#include <ctype.h>

#include "matrix.h"
#include "row.h"
#include "rref.h"

Matrix* try_rref(Dict* matrix_dict, Matrix* m, char* matrix_identifier) {
    /* some sort of error occurred when converting to RREF */
    /* TODO make sure if it fails, the original is still in good state */
    if((m = rref(m, matrix_identifier)) == NULL) {
        if(matrix_identifier == NULL)
            printf("Error: could not reduce matrix\n");
        else
            printf("Error: could not reduce matrix %s\n", matrix_identifier);
        return NULL;
    }

    /* some sort of error putting into dict */
    if(dict_add(matrix_dict, m->name, m) == NULL) {
       printf("Error: try again with a different matrix name\n");
       matrix_destroy(m);
       return NULL;
    }

    return m;
}

/* expects a string (arg1, arg2,..., argn) */
void rref_handler(Dict* matrix_dict, char* line) {
   unsigned i;
   Matrix* m; 
   char new_identifier[MAXIDENTIFIER];

   if(*line++ != '(') {
       printf("Usage: rref(matrix, optional:new_name)\n");
       return;
   }

   /* could not get matrix from dict */
   if((m = try_get_matrix(matrix_dict, &line)) == NULL) {
       return;
   }

   /* skip whitespace after first arg */
   while(*line == ' ' || *line == '\t')
       line++;

   /* no new identifier for copied matrix */
   if(*line == ')') {
       try_rref(matrix_dict, m, NULL);
       return;
   }

   /* error */
   if(*line != ',') {
       printf("Usage: rref(matrix, copy_name)\n");
       return;
   }

   line++;

   /* skip whitespace after comma */
   while(*line == ' ' || *line == '\t')
       line++;
   
   for(i = 0; i < MAXIDENTIFIER - 1 && isalnum(*line); i++)
       new_identifier[i] = *line++;
   new_identifier[i] = '\0';

   /* skip whitespace after second arg */
   while(*line == ' ' || *line == '\t')
       line++;

   /* error */
   if(*line != ')') {
       printf("Usage: rref(matrix, copy_name)\n");
       return;
   }

   try_rref(matrix_dict, m, new_identifier);
}

static unsigned left_most_nz_row(Matrix* m, unsigned cur_row, unsigned* left_most_col) {
    unsigned row_i, col_i, left_most_row;
    Row* row;

    *left_most_col = m->ncols;
    left_most_row = cur_row;
    for(row_i = cur_row; row_i < m->nrows; row_i++) {
        row = m->rows + row_i;
        for(col_i = 0; col_i < m->ncols; col_i++)
            if(row->vals[col_i]) { /* first non zero in row */
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
    Row *tmp = m->rows + i;
    m->rows[i] = m->rows[j];
    m->rows[j] = *tmp;
}

static void make_pivot(Row* r, unsigned cur_col) {
    unsigned i;
    for(i = cur_col + 1; i < r->len; i++)
        r->vals[i] /= r->vals[cur_col];
    r->vals[cur_col] = 1;
    r->pivot = cur_col;
}

static void reduce_below(Matrix* m, unsigned row_i) {
    unsigned col_i, below_row_i;
    double scalar;
    unsigned pivot;
    Row *row = NULL, *below_row = NULL;

    row = m->rows + row_i;
    pivot = row->pivot;
    for(below_row_i = row_i+1; below_row_i < m->nrows; below_row_i++) {
        below_row = m->rows + below_row_i;
        scalar = below_row->vals[pivot];
        for(col_i = pivot; col_i < m->ncols; col_i++)
            below_row->vals[col_i] -= scalar * row->vals[col_i];
    }
}

static void reduce_above(Matrix* m, unsigned row_i) {
    unsigned above_row_i, col_i;
    unsigned pivot;
    double scalar;
    Row *row = NULL, *above_row = NULL;

    /* for each row */
    /* get pivot pos */
    /* for each row above this row */
    /*    use scalar mult to reduce each column */
    row = m->rows + row_i;
    pivot = row->pivot;
    for(above_row_i = 0; above_row_i < row_i; above_row_i++) {
        above_row = m->rows + above_row_i;

        scalar = above_row->vals[pivot];
        for(col_i = pivot; col_i < m->ncols; col_i++)
            above_row->vals[col_i] -= scalar * row->vals[col_i];
    }
}

static Matrix* ref(Matrix* m, char* identifier) {
    Matrix* copy = matrix_copy(m, identifier);
    Row* r;

    /* cur_row -> index of row we are interested in swapping 
     * mov_row -> index or row we want to swap into cur_row
     *
     * if cur_col == copy->ncols, then its a zero row
     */
    unsigned cur_row, cur_col, move_row;

    for(cur_row = 0;
            (move_row = left_most_nz_row(copy, cur_row, &cur_col)) < copy->nrows;
            cur_row++) {
        /* place left most row in next highest position */
        if(cur_row != move_row)
            swap(m, cur_row, move_row);

        /* don't operate on a zero row */
        if(cur_col == copy->ncols)
            continue;

        r = copy->rows + cur_row;
        /* make first element a pivot */
        make_pivot(r, cur_col);

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
Matrix* rref(Matrix* m, char* identifier) {
    unsigned row_i;
    Matrix* copy = ref(m, identifier);
    for(row_i = 0; row_i < copy->nrows; row_i++) {
        if(copy->rows[row_i].pivot < copy->ncols) /* is this a non zero row */
            reduce_above(copy, row_i);
    }
    return copy;
}
