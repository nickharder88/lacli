#include <stdlib.h>

#include "inverse.h"
#include "funcs.h"
#include "I.h"
#include "aug.h"
#include "rref.h"

void inverse_handler(Dict* matrix_dict, char* line) {
    char *identifier;
    Matrix **marr;

    if((marr = parse_args(matrix_dict, line, 1, &identifier)) == NULL) {
       printf("Usage: inverse(matrix, optional:newname)\n");
       return;
    }

    if(identifier == NULL)
        identifier = (*marr)->name;
    try_inverse(matrix_dict, *marr, identifier);
}

Matrix* try_inverse(Dict* matrix_dict, Matrix* m, char* identifier) {
    if((m = inverse(m, identifier)) == NULL) {
       printf("Error: could not create inverse matrix\n");
       return NULL;
    }

    /* some sort of error putting into dict */
    if(dict_add(matrix_dict, m->name, m) == NULL) {
       printf("Error: try again with a different matrix name\n");
       matrix_destroy(m);
       return NULL;
    }

    return m;
}

Matrix* inverse(Matrix* m, char* identifier) {
    Matrix *copy, *m_aug, *In, *m_aug_rref;

    if(m->ncols != m->nrows) {
        printf("Usage: matrix must be square.\n");
        return NULL;
    }

    copy = matrix_copy(m, "copy");
    In = I(m->nrows, "tmp");

    m_aug = aug(copy, In, copy->name);
    m_aug_rref = rref(m_aug, identifier);
    matrix_slice_after(m_aug_rref, m->ncols);

    free(copy);
    free(In);
    free(m_aug);
    return m_aug_rref;
}
