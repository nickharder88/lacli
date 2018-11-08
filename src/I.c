#include <ctype.h>
#include <stdlib.h>

#include "I.h"

void I_handler(Dict* matrix_dict, char* line) {
    char new_identifier[MAXIDENTIFIER];
    unsigned i;
    unsigned nrows = 0;

    if(*line++ != '(') {
       printf("Usage: I(number_rows, name)\n");
       return;
    }

    /* skip whitespace */
    while(*line == ' ')
        line++;

    for(; isdigit(*line); line++) {
        nrows *= 10;
        nrows += (*line - '0');
    }

    if(nrows == 0) {
        printf("Usage: Matrix must have more than 0 rows.\n");
        return;
    }

    /* skip whitespace */
    while(*line == ' ')
        line++;

    if(*line != ',') {
        printf("Usage: I(number_rows, name)\n");
        return;
    }

    /* skip , */
    line++;

    for(i = 0; i < MAXIDENTIFIER - 1 && isalnum(*line); line++, i++)
        new_identifier[i] = *line;
    new_identifier[i] = '\0';

    /* skip whitespace */
    while(*line == ' ')
        line++;

    if(*line != ')') {
        printf("Usage: I(number_rows, name)\n");
        return;
    }

    try_I(matrix_dict, nrows, new_identifier);
}

Matrix* try_I(Dict* matrix_dict, unsigned nrows, char* identifier) {
    Matrix* m;
    if((m = I(nrows, identifier)) == NULL) {
       printf("Error: could not create Identity matrix\n");
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

Matrix* I(unsigned nrows, char* identifier) {
    Matrix* m;
    Row* r;
    unsigned row_i, col_i;

    m = matrix_create(identifier);
    m->rows = malloc(nrows * sizeof(struct Row));
    m->ncols = nrows;
    m->nrows = nrows;
    for(row_i = 0; row_i < nrows; row_i++) {
        r = m->rows + row_i;
        r->len = nrows;
        r->vals = malloc(nrows * sizeof(double));
        for(col_i = 0; col_i < nrows; col_i++)
            r->vals[col_i] = 0;
        r->vals[row_i] = 1;
        r->pivot = row_i;
    }
    
    return m;
}
