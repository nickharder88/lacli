#include <float.h>
#include <math.h>
#include <stdio.h>

#include "util.h"

#define EPSILON 0.0001
#define SIG 0.00000001

/* returns 0 if approximately equal */
int double_cmp(double d1, double d2) {
    int val = fabs(d1 - d2) < EPSILON;

    double max = 1;
    if(fabs(d1) > max)
        max = fabs(d1);
    else if(fabs(d2) > max)
        max = fabs(d2);

    if(fabs(d1 - d2) < EPSILON * max) {
        return 0;
    }

    return 1;
}

void double_print(double* vals, unsigned len) {
    unsigned i;
    putchar('\t');
    for(i = 0; i < len - 1; i++)
        printf("%g\t", vals[i]);
    printf("%g\n", vals[i]);
}

/* rounds to closest integer if value */
double round_if_insignificant(double val) {
    if(val == 0)
        return 0;
    if(fabs(val - round(val)) < SIG)
        return round(val);
    return val;
}

double gcf(double d1, double d2) {
    if(d1 < d2)
        return gcf(d2, d1);

    if(round_if_insignificant(d2) == 0)
        return d1;

    return gcf(d2, d1 - floor(d1 / d2) * d2);
}
