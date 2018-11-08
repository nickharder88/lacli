#include <ctype.h>
#include <stdlib.h>

#include "aug.h"
#include "funcs.h"

void aug_handler(Dict* matrix_dict, char* line) {
    char *identifier;
    Matrix **marr;

    if((marr = parse_args(matrix_dict, line, 2, &identifier)) == NULL) {
       printf("Usage: aug(matrix1, matrix2, optional:newname)\n");
       return;
    }

    if(identifier == NULL)
        identifier = (*marr)->name;
    try_aug(matrix_dict, marr[0], marr[1], identifier);
}

Matrix* try_aug(Dict* matrix_dict, Matrix* m1, Matrix* m2, char* identifier) {
    Matrix* m;
    if((m = aug(m1, m2, identifier)) == NULL) {
       printf("Error: could not create augmented matrix\n");
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

Matrix* aug(Matrix* m1, Matrix* m2, char* identifier) {
    Matrix *m;
    Row *row, *m1_row, *m2_row;
    unsigned row_i, len, col_i;

    if(m1->nrows != m2->nrows) {
        printf("Error: matrices must have the same number of rows.\n");
        return NULL;
    }

    if(identifier == NULL) {
        printf("Error: matrix must have non empty identifier.\n");
        return NULL;
    }

    len = m1->ncols + m2->ncols;
    m = matrix_create_dim(identifier, m1->nrows, len);
    m->rows = malloc(m->nrows * sizeof(struct Row));

    for(row_i = 0; row_i < m->nrows; row_i++) {
        row = row_create(len, m->ncols);
        m->rows[row_i] = *row;
        m1_row = m1->rows + row_i;
        m2_row = m2->rows + row_i;

        for(col_i = 0; col_i < m1->ncols; col_i++)
            row->vals[col_i] = m1_row->vals[col_i];
        for(col_i = 0; col_i < m2->ncols; col_i++)
            row->vals[col_i+m1->ncols] = m2_row->vals[col_i];
    }

    return m;
}
