#ifndef LITERAL_H
#define LITERAL_H

#include "cplx.h"

typedef struct Literal {
    enum {
        REAL, CPLX
    } type;

    double real;
    Cplx* cplx;
} Literal;

Literal* literal_create_real(double val);
Literal* literal_create_cplx(Cplx* val);

char literal_cmp(Literal* l1, Literal* l2);
void literal_print(Literal* lit);
void literal_print_multiple(Literal** literals, unsigned length);
void literal_destroy(Literal* lit);

#endif
