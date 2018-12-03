#include <stdlib.h>

#include "rval.h"

void rval_destroy(Rval* val) {
    if(val->type == RMATRIX) {
        matrix_destroy(val->value.matrix);
    } else if(val->type == RLITERAL_ARRAY) {
        free(val->value.array.literal_array);
    }
    free(val);
}

void rval_print(Rval* val) {
    unsigned i;

    switch(val->type) {
        case RLITERAL:
            printf("%f\n", val->value.literal);
            break;
        case RLITERAL_ARRAY:
            for(i = 0; i < val->value.array.length - 1; i++)
                printf("%f\t", val->value.array.literal_array[i]);
            printf("%f\n", val->value.array.literal_array[val->value.array.length - 1]);
            break;
        case RMATRIX:
            matrix_print(val->value.matrix);
            break;
        case RNIL:
            printf("NULL\n");
            break;
        case RBOOLEAN:
            if(val->value.boolean == TRUE)
                printf("TRUE\n");
            else
                printf("FALSE\n");
            break;
        default:
            break;
    }
}

Rval* rval_make_literal(double val) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RLITERAL;
    rval->value.literal = val;
    return rval;
}

Rval* rval_make_matrix(Matrix* m) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RMATRIX;
    rval->value.matrix = m;
    return rval;
}

Rval* rval_make_nil(void) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RNIL;
    return rval;
}

Rval* rval_make_literal_array(double* arr, unsigned len) {
    unsigned i;
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RLITERAL_ARRAY;

    rval->value.array.length = len;
    rval->value.array.literal_array = malloc(len * sizeof(double));
    for(i = 0; i < len; i++)
        rval->value.array.literal_array[i] = arr[i];

    return rval;
}

Rval* rval_make_boolean(Boolean boolean) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RBOOLEAN;
    rval->value.boolean = boolean;
    return rval;
}
