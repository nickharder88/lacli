#include <stdlib.h>

#include "rval.h"

void rval_destroy(Rval* val) {
    unsigned i;

    switch(val->type) {
        case RMATRIX:
            matrix_destroy(val->value.matrix);
            break;
        case RMATRIX_ARRAY:
            for(i = 0; i < val->value.array.length; i++)
                matrix_destroy(val->value.array.matrix_array[i]);
            free(val->value.array.matrix_array);
            break;
        case RLITERAL_ARRAY:
            free(val->value.array.literal_array);
            break;
        default:
            break;
    }
    free(val);
}

void rval_print(Rval* val) {
    unsigned i;

    switch(val->type) {
        case RLITERAL:
            printf("%g\n", val->value.literal);
            break;
        case RLITERAL_ARRAY:
            for(i = 0; i < val->value.array.length - 1; i++)
                printf("%g\t", val->value.array.literal_array[i]);
            printf("%g\n", val->value.array.literal_array[val->value.array.length - 1]);
            break;
        case RMATRIX:
            matrix_print(val->value.matrix);
            break;
        case RMATRIX_ARRAY:
            matrix_print_multiple(val->value.array.matrix_array, val->value.array.length);
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

Rval* rval_make_matrix_array(Matrix** marr, unsigned length) {
    unsigned i;
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RMATRIX_ARRAY;

    rval->value.array.length = length;
    rval->value.array.matrix_array = malloc(length * sizeof(struct Matrix *));
    for(i = 0; i < length; i++)
        rval->value.array.matrix_array[i] = marr[i];

    return rval;
}
