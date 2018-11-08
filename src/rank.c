#include <stdlib.h>

#include "rank.h"
#include "funcs.h"
#include "rref.h"

void rank_handler(Dict* matrix_dict, char* line) {
    unsigned mrank;
    Matrix **marr;

    if((marr = parse_args(matrix_dict, line, 1, NULL)) == NULL) {
       printf("Usage: rank(matrix)\n");
       return;
    }

    mrank = rank(*marr);
    printf("%u\n", mrank);
}

unsigned rank(Matrix* m) {
    unsigned mrank = 0, i;

    Matrix* m_rref = rref(m, "tmp");
    for(i = 0; i < m_rref->nrows; i++)
        if(m_rref->rows[i].pivot < m_rref->ncols)
            mrank++;

    free(m_rref);
    return mrank;
}
