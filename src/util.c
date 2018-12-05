#include <float.h>
#include <math.h>
#include "util.h"

#define EPSILON 0.0001
#define SIG 0.00000001

/* returns 0 if approximately equal */
int cmp_double(double d1, double d2) {
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

/* rounds to closest integer if value */
double round_if_insignificant(double val) {
    if(val == 0)
        return 0;
    if(fabs(val - round(val)) < SIG)
        return round(val);
    return val;
}
