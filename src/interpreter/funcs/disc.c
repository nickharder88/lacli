#include "disc.h"
#include "det.h"
#include "trace.h"

Rval* discriminant_handler(Rval** args, unsigned nargs) {
    if(nargs != 1 || args[0]->type != RMATRIX) {
        printf("Usage: discriminant(matrix)\n");
        return NULL;
    }

    return discriminant(args[0]->value.matrix);
}

Rval* discriminant(Matrix* m) {
    double disc;
    Rval *m_trace, *m_det;

    m_trace = trace(m);
    m_det = det(m);

    disc = discriminant_solve(m_trace->value.literal, m_det->value.literal);

    rval_destroy(m_trace);
    rval_destroy(m_det);
    return rval_make_literal(disc);
}

double discriminant_solve(double m_trace, double m_det) {
    return m_trace * m_trace - 4 * m_det;
}
