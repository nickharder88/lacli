#include "evec.h"
#include "I.h"
#include "aug.h"
#include "rref.h"

Matrix* evec(Matrix* m, double eval, char* identifier) {
    Matrix *m_evec, *m_eval, *m_sub, *m_zero, *m_aug;

    /* (C - eval*I2)v = 0*/
    m_eval = I(2, "tmp");
    matrix_multiply_constant(m_eval, eval);
    m_sub = matrix_subtract("sub", m, m_eval);
    m_zero = matrix_create_zero("zero", 2, 1);
    m_aug = aug(m, m_zero, "aug");
    m_evec = rref(m_aug, identifier);

    matrix_destroy(m_eval);
    matrix_destroy(m_sub);
    matrix_destroy(m_zero);
    matrix_destroy(m_aug);
    return m_evec;
}
