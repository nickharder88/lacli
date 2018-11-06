#include <ctype.h>

#include "matrix.h"
#include "row.h"

static Matrix* rref(Matrix* m, char* identifier);

/* expects a string (arg1, arg2,..., argn) */
void rref_handler(Dict* matrix_dict, char* line) {
   unsigned i;
   Matrix* m; 
   char matrix_identifier[MAXIDENTIFIER];
   char new_identifier[MAXIDENTIFIER];

   if(*line++ != '(') {
       printf("Usage: rref(matrix, optional:new_name)\n");
       return;
   }


   for(i = 0; i < MAXIDENTIFIER - 1 && isalnum(*line); i++)
       matrix_identifier[i] = *line++;
   matrix_identifier[i] = '\0';

   /* skip whitespace after first arg */
   while(*line == ' ' || *line == '\t')
       line++;

   /* no new identifier for copied matrix */
   if(*line == ')') {
       /* matrix does not exist */
       if((m = dict_get(matrix_dict, matrix_identifier)) == NULL) {
           printf("Error: matrix %s does not exist.\n", matrix_identifier);
           return;
       }

       /* some sort of error occurred when converting to RREF */
       /* TODO make sure if it fails, the original is still in good state */
       if((m = rref(m, NULL)) == NULL) {
           printf("Error: could not reduce matrix %s\n", matrix_identifier);
           return;
       }

       /* some sort of error putting into dict */
       if(dict_add(matrix_dict, matrix_identifier, m) == NULL) {
           printf("Error: try again with a different matrix name\n");
           matrix_destroy(m);
           return;
       }

       /* successful */
       return;
   }

   /* TODO pick up from here */

   /* error */
   if(*line != ',') {
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

   }


}

static unsigned left_most_nz_row(Matrix* m, unsigned cur_row, unsigned* left_most_col) {
    unsigned i, col, left_most_row;

    *left_most_col = m->ncols;
    left_most_row = cur_row;
    for(i = cur_row; i < m->nrows; i++) {
        for(col = 0; col < m->ncols; col++)
            if(m->rows[i].vals[col]) { /* first non zero in row */
                if(*left_most_col > col) {
                    *left_most_col = col;
                    left_most_row = i;
                }
                break;
            }
    }

    return left_most_row;
}

static void swap(Matrix* m, unsigned i, unsigned j) {
    Row tmp = m->rows[i];
    m->rows[i] = m->rows[j];
    m->rows[j] = tmp;
}

static void make_pivot(Row* r, unsigned cur_col) {
    unsigned i;
    for(; i < r->len; i++)
        r->vals[i] /= r->vals[cur_col];
    r->vals[cur_col] = 1;
}

static void reduce_below(Matrix* m, unsigned row, unsigned cur_col) {
    unsigned i, j, next_row = row + 1;
    double quotient;
    Row r = m->rows[row];

    for(i = next_row; i < m->nrows; i++) {
        r = m->rows[i];
        if(!(r.vals[cur_col]))
            continue;
        quotient = r.vals[cur_col] / m->rows[next_row].vals[cur_col];
    }
}

static Matrix* ref(Matrix* m, char* identifier) {
    Matrix* copy = matrix_copy(m, identifier);
    Row* r;

    /* cur_row -> index of row we are interested in swapping 
     * work_row -> index or row we want to swap into cur_row
     */
    unsigned cur_row = 0, cur_col, move_row;

    while((move_row = left_most_nz_row(copy, cur_row, &cur_col)) < copy->nrows) {
        /* place left most row in next highest position */
        swap(m, cur_row, move_row);
        *r = copy->rows[cur_row];

        /* make first element a pivot */
        make_pivot(r, cur_col);

        /* reduce all the rows below to 0 */
        reduce_below(m, cur_row, cur_col);

        cur_row++;
    }

    return copy;
}

/*
 * Creates a copy of matrix m and puts it in rref
 * if identifier is NULL, then it replaces m with the copy
 */
static Matrix* rref(Matrix* m, char* identifier) {
    Matrix* copy = ref(m, identifier);
    // 7. determine all the leading ones in the row echelon form
    // 8. determine the right most columb containing a leading one
    // 9. use scaled addition to reduce all nonzero entries above the leading one in the pivot column
    // 10. if no more columbs containing leding ones go to 12
    // 11. apply 8-10
    // 12. the resulting matrix is in rref

    return copy;
}
