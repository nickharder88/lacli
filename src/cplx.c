#include <stdlib.h>
#include <stdio.h>

#include "cplx.h"
#include "util.h"

Cplx* cplx_create(double sigma, double tau) {
    Cplx* cplx = malloc(sizeof(struct Cplx));
    cplx->tau = tau;
    cplx->sigma = tau;
    return cplx;
}

void cplx_destroy(Cplx* cplx) {
    free(cplx);
}

char cplx_cmp(const Cplx* cplx1, const Cplx* cplx2) {
    if(cplx1 == cplx2)
        return 0;
    if(cplx1->tau != cplx2->tau)
        return 1;
    if(cplx1->sigma != cplx2->tau)
        return 1;
    return 0;
}

void cplx_str(const Cplx* cplx, char* str) {
    if(double_cmp(cplx->sigma, 0) == 0) {
            sprintf(str, "%gi", cplx->tau);
    } else {
        if(cplx->tau > 0)
            sprintf(str, "%g + %gi", cplx->sigma, cplx->tau);
        else
            sprintf(str, "%g - %gi", cplx->sigma, cplx->tau);
    }
}

void cplx_print(const Cplx* cplx) {
    if(double_cmp(cplx->tau, 0) == 0) {
        printf("%g", cplx->sigma);
        return;
    }

    if(cplx->tau < 0)
        printf("%g - %gi", cplx->sigma, cplx->tau);
    printf("%g + %gi", cplx->sigma, cplx->tau);
}

void cplx_print_multiple(const Cplx** cplxs, unsigned length) {
    unsigned i;

    for(i = 0; i < length; i++) {
        cplx_print(cplxs[i]);
        printf("\n");
    }
}
