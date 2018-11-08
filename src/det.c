#include "funcs.h"
#include "det.h"

void det_handler(Dict* matrix_dict, char* line) {
    double mdet;
    Matrix **marr;

    if((marr = parse_args(matrix_dict, line, 1, NULL)) == NULL) {
       printf("Usage: det(matrix)\n");
       return;
    }

    mdet = det(*marr);
    printf("%f\n", mdet);

}

double det(Matrix* m) {
    if(m->nrows != 2 || m->ncols != 2) {
        printf("Usage: can only get determinant of 2x2 matrices.\n");
        return 0;
    }

    return (m->rows[0].vals[0] * m->rows[1].vals[1])
           - (m->rows[0].vals[1] * m->rows[1].vals[0]);
}
