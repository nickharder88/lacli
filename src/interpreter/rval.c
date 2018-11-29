#include <stdlib.h>

#include "rval.h"

void rval_destroy(Rval* val) {
    if(val->type == RMATRIX) {
        matrix_destroy(val->value.matrix);
    }
    free(val);
}

void rval_print(Rval* val) {
    if(val->type == RLITERAL) {
        printf("%f\n", val->value.literal);
        return;
    } 

    matrix_print(val->value.matrix);
}

Rval* make_rval_literal(double val) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RLITERAL;
    rval->value.literal = val;
    return rval;
}

Rval* make_rval_matrix(Matrix* m) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RMATRIX;
    rval->value.matrix = m;
    return rval;
}


