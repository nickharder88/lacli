#include <string.h>
#include <stdlib.h>

#include "rval.h"
#include "../util.h"

void rval_destroy(Rval* val) {
    unsigned i;

    /* don't destroy values in the environment */
    if(val->in_env)
        return;

    switch(val->type) {
        case RMATRIX:
            if(val->value.matrix != NULL)
                matrix_destroy(val->value.matrix);
            break;
        case RMATRIX_ARRAY:
            if(val->value.array.matrix_array != NULL) {
                for(i = 0; i < val->value.array.length; i++)
                    matrix_destroy(val->value.array.matrix_array[i]);
                free(val->value.array.matrix_array);
            }
            break;
        case RLITERAL_ARRAY:
            if(val->value.array.literal_array != NULL)
                free(val->value.array.literal_array);
            break;
        case REQU:
            equation_destroy(val->value.equation);
            break;
        case RSTR:
            if(val->value.str != NULL)
                free(val->value.str);
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
        case REQU:
            equation_print(val->value.equation);
            break;
        case RSTR:
            printf("%s\n", val->value.str);
        default:
            break;
    }
}

char rval_cmp(Rval* val1, Rval* val2) {
    unsigned i;
    Matrix **marr1, **marr2;
    double *darr1, *darr2;

    if(val1->type != val2->type)
        return 1;

    switch(val1->type) {
        case RMATRIX:
            return matrix_cmp(val1->value.matrix, val2->value.matrix);
        case RMATRIX_ARRAY:
            if(val1->value.array.length != val2->value.array.length)
                return 1;

            marr1 = val1->value.array.matrix_array;
            marr2 = val2->value.array.matrix_array;
            for(i = 0; i < val1->value.array.length; i++)
                if(matrix_cmp(marr1[i], marr2[i]))
                    return 1;
            return 0;
        case RLITERAL:
            return double_cmp(val1->value.literal, val2->value.literal);
        case RLITERAL_ARRAY:
            if(val1->value.array.length != val2->value.array.length)
                return 1;

            darr1 = val1->value.array.literal_array;
            darr2 = val2->value.array.literal_array;
            for(i = 0; i < val1->value.array.length; i++)
                if(darr1[i] != darr2[i])
                    return 1;
            return 0;
        case RBOOLEAN:
            return val1->value.boolean != val2->value.boolean;
        case REQU:
            return equation_cmp(val1->value.equation, val2->value.equation);
        case RSTR:
            return strcmp(val1->value.str, val2->value.str);
        default:
            break;
    }

    return 0;
}

Rval* rval_make_literal(double val) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RLITERAL;
    rval->value.literal = val;
    rval->in_env = 0;
    return rval;
}

Rval* rval_make_matrix(Matrix* m) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RMATRIX;
    rval->value.matrix = m;
    rval->in_env = 0;
    return rval;
}

Rval* rval_make_nil(void) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RNIL;
    rval->in_env = 0;
    return rval;
}

Rval* rval_make_literal_array(double* arr, unsigned len) {
    unsigned i;
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RLITERAL_ARRAY;
    rval->in_env = 0;

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
    rval->in_env = 0;
    return rval;
}

Rval* rval_make_matrix_array(Matrix** marr, unsigned length) {
    unsigned i;
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = RMATRIX_ARRAY;
    rval->in_env = 0;

    rval->value.array.length = length;
    rval->value.array.matrix_array = malloc(length * sizeof(struct Matrix *));
    for(i = 0; i < length; i++)
        rval->value.array.matrix_array[i] = marr[i];

    return rval;
}

Rval* rval_make_equ(Equation* equ) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->in_env = 0;
    rval->type = REQU;
    rval->value.equation = equ;
    return rval;
}

Rval* rval_make_str(char* str) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->in_env = 0;
    rval->type = RSTR;
    rval->value.str = strdup(str);
    return rval;
}
