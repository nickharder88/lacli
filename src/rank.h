#ifndef RANK_H
#define RANK_H

#include "matrix.h"

void rank_handler(Dict* matrix_dict, char* line);
unsigned try_rank(Dict* matrix_dict, Matrix* m);
unsigned rank(Matrix* m);

#endif
