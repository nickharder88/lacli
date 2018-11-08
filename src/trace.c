#include "trace.h"
#include "funcs.h"

void trace_handler(Dict* matrix_dict, char* line) {
    double mtrace;
    Matrix **marr;

    if((marr = parse_args(matrix_dict, line, 1, NULL)) == NULL) {
       printf("Usage: trace(matrix)\n");
       return;
    }

    mtrace = trace(*marr);
    printf("%.2f\n", mtrace);
}

double trace(Matrix* m) {
    unsigned i;
    double val = 0;

    for(i = 0; i < m->nrows; i++)
        val += m->rows[i].vals[i];
    return val;
}
