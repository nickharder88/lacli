#include <check.h>
#include "../src/matrix.h"

Matrix* matrix_add(Matrix* a, Matrix* b);
Matrix* matrix_subtract(Matrix* a, Matrix* b);
Matrix* matrix_multiply_constant(Matrix* m, double val);
Matrix* matrix_multiply(Matrix* a, Matrix* b);
Matrix* matrix_subtract(Matrix *m1, Matrix *m2);

START_TEST(test_matrix_create_dim) {
    Matrix *m = matrix_create_dim(2, 2);
    ck_assert(m->nrows == 2);
    ck_assert(m->ncols == 2);
}
END_TEST

START_TEST(test_matrix_cmp_equal) {
    unsigned row_i, col_i;
    Matrix *m1, *m2;
    Matrix* row;

    m1 = matrix_create_dim(2, 2);
    for(row_i = 0; row_i < m1->nrows; row_i++) {
        row = m1->values.rows + row_i;
        for(col_i = 0; col_i < m1->ncols; col_i++)
            row->values.literals[col_i] = 1;
    }

    m1 = matrix_create_dim(2, 2);
    for(row_i = 0; row_i < m1->nrows; row_i++) {
        row = m1->values.rows + row_i;
        for(col_i = 0; col_i < m1->ncols; col_i++)
            row->values.literals[col_i] = 1;
    }

    ck_assert(matrix_cmp(m1, m2) == 0);
}
END_TEST

START_TEST(test_matrix_cmp_not_equal) {
    unsigned row_i, col_i;
    Matrix *m1, *m2;
    Matrix* row;

    m1 = matrix_create_dim(2, 2);
    for(row_i = 0; row_i < m1->nrows; row_i++) {
        row = m1->values.rows + row_i;
        for(col_i = 0; col_i < m1->ncols; col_i++)
            row->values.literals[col_i] = 1;
    }

    m1 = matrix_create_dim(2, 2);
    for(row_i = 0; row_i < m1->nrows; row_i++) {
        row = m1->values.rows + row_i;
        for(col_i = 0; col_i < m1->ncols; col_i++)
            row->values.literals[col_i] = 1;
        row->values.literals[1] = 0;
    }

    ck_assert(matrix_cmp(m1, m2) != 0);
}
END_TEST


Suite* matrix_suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("matrix");
    tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_matrix_create_dim);
    tcase_add_test(tc_core, test_matrix_cmp_equal);
    tcase_add_test(tc_core, test_matrix_cmp_not_equal);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite* s;
    SRunner *sr;

    s = matrix_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    printf("number failed: %d\n", number_failed);

    return 0;
}
