#include <string.h>
#include <stdlib.h>

#include "rval.h"
#include "../util.h"

void rval_destroy(Rval* val) {
    unsigned i;

    switch(val->type) {
        case RMATRIX:
            if(val->value.matrix)
                matrix_destroy(val->value.matrix);
            break;
        case RMATRIX_ARRAY:
        case RLITERAL_ARRAY:
            if(val->value.array != NULL)
                array_destroy(val->value.array);
            break;
        case REQU:
            equation_destroy(val->value.equation);
            break;
        case RCPLX:
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
        case RMATRIX_ARRAY:
            array_print(val->value.array);
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
        case REQU:
            equation_print(val->value.equation);
        default:
            break;
    }
}

char rval_cmp(Rval* val1, Rval* val2) {
    unsigned i;
    Matrix **marr1, **marr2;
    double *darr1, *darr2;
    Array* arr;

    if(val1->type != val2->type)
        return 1;

    switch(val1->type) {
        case RMATRIX:
            return matrix_cmp(val1->value.matrix, val2->value.matrix);
        case RMATRIX_ARRAY:
        case RLITERAL_ARRAY:
            return array_cmp(val1->value.array, val2->value.array);

        case RLITERAL:
            return double_cmp(val1->value.literal, val2->value.literal);
        case RBOOLEAN:
            return val1->value.boolean != val2->value.boolean;
        case REQU:
            return equation_cmp(val1->value.equation, val2->value.equation);
        case RCPLX:
            break;
        default:
            break;
    }

    return 0;
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

Rval* rval_make_literal_array(double* darr, unsigned len) {
    unsigned i;
    Rval* rval;
    Array* arr;

    rval = malloc(sizeof(struct Rval));
    arr = array_create_literal(darr, len);

    rval->type = RLITERAL_ARRAY;
    rval->value.array = arr;
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
    Array* arr;
    Rval* rval;

    rval = malloc(sizeof(struct Rval));
    arr = array_create_matrix(marr, length);
    /* TODO shallow or deep copy ?*/
    rval->type = RMATRIX_ARRAY;
    rval->value.array = arr;
    return rval;
}

Rval* rval_make_equ(Equation* equ) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = REQU;
    rval->value.equation = equ;
    return rval;
}

Rval* rval_make_cplx(Cplx* cplx) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RCPLX;
    rval->value.cplx = cplx;
    return rval;
}

Rval* rval_make_cplx_array(Cplx** cplxs, unsigned length) {
    unsigned i;
    Array* arr;
    Rval* rval;

    rval = malloc(sizeof(struct Rval));
    arr = array_create_cplx(cplxs, length);
    /* TODO shallow or deep copy ?*/
    rval->type = RCPLX_ARRAY;
    rval->value.array = arr;
    return rval;

}

