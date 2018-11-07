#include <ctype.h>
#include <stdlib.h>

#include "aug.h"
#include "funcs.h"

void aug_handler(Dict* matrix_dict, char* line) {
    char new_identifier[MAXIDENTIFIER];
    unsigned i;
    Matrix *m1, *m2;

    if(*line++ != '(') {
       printf("Usage: aug(matrix1, matrix2, optional:newname)\n");
       return;
    }

    /* could not get matrix */
    if((m1 = try_get_matrix(matrix_dict, &line)) == NULL) {
        return;
    }

    /* skip whitespace */
    while(*line == ' ' || *line == '\t')
        line++;

    if(*line != ',') {
        printf("Usage: aug(matrix1, matrix2, optional:newname)\n");
        return;
    }

    line++;

    /* skip whitespace */
    while(*line == ' ' || *line == '\t')
        line++;

    if((m2 = try_get_matrix(matrix_dict, &line)) == NULL) {
        return;
    }

    /* skip whitespace */
    while(*line == ' ' || *line == '\t')
        line++;

    if(*line == ')') {
        try_aug(matrix_dict, m1, m2, NULL);
        return;
    } else if(*line != ',') {
        printf("Usage: aug(matrix1, matrix2, optional:newname)\n");
        return;
    }

    /* skip , */
    line++;

    /* skip whitespace */
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
        printf("Usage: aug(matrix1, matrix2, optional:newname)\n");
    }

    try_aug(matrix_dict, m1, m2, new_identifier);
}

Matrix* try_aug(Dict* matrix_dict, Matrix* m1, Matrix* m2, char* identifier) {
    Matrix* m;
    if((m = aug(matrix_dict, m1, m2, identifier)) == NULL) {
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

Matrix* aug(Dict* matrix_dict, Matrix* m1, Matrix* m2, char* identifier) {
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
