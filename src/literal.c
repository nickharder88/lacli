#include <stdlib.h>
#include <stdio.h>

#include "literal.h"
#include "util.h"

Literal* literal_create_real(double val) {
    Literal* lit = malloc(sizeof(struct Literal));
    lit->real = val;
    lit->type = REAL;
    return lit;
}

Literal* literal_create_cplx(Cplx* val) {
    Literal* lit = malloc(sizeof(struct Literal));
    lit->cplx = val;
    lit->type = CPLX;
    return lit;
}

char literal_cmp(Literal* l1, Literal* l2) {
    if(l1->type != l2->type)
        return 1;

    if(l1->type == REAL)
        return double_cmp(l1->real, l2->real);
    return cplx_cmp(l2->cplx, l2->cplx);
}

void literal_str(Literal* lit, char* str, unsigned length) {
    unsigned i;
    if(lit->type == REAL)
        sprintf(str, "%g", lit->real);
    else
        cplx_str(lit->cplx, str);
}

void literal_print(Literal* lit) {
    if(lit->type == REAL)
        printf("%g", lit->real);
    else
        cplx_print(lit->cplx);
}

void literal_print_multiple(Literal** literals, unsigned length) {
    unsigned i;
    for(i = 0; i < length; i++) {
        literal_print(literals[i]);
        printf("\n");
    }
}

void literal_destroy(Literal* lit) {
    unsigned i;
    if(lit->type == CPLX)
        cplx_destroy(lit->cplx);
    free(lit);
}
