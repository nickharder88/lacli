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
