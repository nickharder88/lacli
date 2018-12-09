#ifndef CPLX_H
#define CPLX_H

/* eigenvalues represented as sigma += tau */
typedef struct Cplx {
    double tau;
    double sigma;
} Cplx;

Cplx* cplx_create(double sigma, double tau);
void cplx_destroy(Cplx* cplx);

char cplx_cmp(const Cplx* cplx1, const Cplx* cplxr2);

void cplx_str(const Cplx* cplx, char* str);
void cplx_print(const Cplx* cplx);
void cplx_print_multiple(const Cplx** cplxs, unsigned length);

#endif
