#include "transpose.h"
#include "funcs.h"

void transpose_handler(Dict* matrix_dict, char* line) {
    char *identifier;
    Matrix **marr;

    if((marr = parse_args(matrix_dict, line, 1, &identifier)) == NULL) {
       printf("Usage: transpose(matrix, optional:newname)\n");
       return;
    }

    if(identifier == NULL)
        identifier = (*marr)->name;
    try_transpose(matrix_dict, *marr, identifier);
}

Matrix* try_transpose(Dict* matrix_dict, Matrix* m, char* identifier) {
    if((m = transpose(m, identifier)) == NULL) {
       printf("Error: could not create transpose matrix\n");
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

Matrix* transpose(Matrix* m, char* identifier) {
    Matrix *t;
    unsigned row_i, col_i;
    unsigned ncols = m->ncols;

    if(m->ncols != m->nrows) {
        printf("Usage: matrix must be square.\n");
        return NULL;
    }

    t = matrix_create_empty(identifier, m->ncols, m->nrows);

    for(row_i = 0; row_i < t->nrows; row_i++)
        for(col_i = 0; col_i < ncols; col_i++)
            t->rows[row_i].vals[col_i] = m->rows[col_i].vals[row_i];

    return t;
}
