#include <math.h>

#include "eval.h"
#include "funcs.h"
#include "aug.h"
#include "I.h"
#include "det.h"
#include "trace.h"

void eval_handler(Dict* matrix_dict, char* line) {
    double eval1, eval2;
    Matrix **marr;

    if((marr = parse_args(matrix_dict, line, 1, NULL)) == NULL) {
       printf("Usage: det(matrix)\n");
       return;
    }

    /* error occurred */
    if(eval(*marr, &eval1, &eval2))
        return;
    printf("%.2f %.2f\n", eval1, eval2);
}

char eval(Matrix* m, double* eval1, double* eval2) {
    /* 0 = y^2 + (trace)y - det(y) */
    /* = (-trace +- sqrt(trace^2 - 4(1)(det)))) /2*/
    double fourac, m_trace, trace_squared;

    fourac = 4 * det(m);
    m_trace = trace(m);
    trace_squared = pow(m_trace, 2);
    if(fourac > trace_squared) {
        printf("Error: complex eigenvalues\n");
        *eval1 = 0;
        *eval2 = 0;
        return 1;
    }

    *eval1 = (-1*m_trace + sqrt(trace_squared - fourac) ) / 2;
    *eval2 = (-1*m_trace - sqrt(trace_squared - fourac) ) / 2;

    return 0;
}
