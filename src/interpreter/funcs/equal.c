#include "equal.h"

Rval* equal_handler(Rval** args, unsigned nargs) {
    if(nargs != 2) {
        printf("Usage: equal(val, val)\n");
        return NULL;
    }
    return equal(args[0], args[1]);
}

Rval* equal(Rval* val1, Rval* val2) {
    if(rval_cmp(val1, val2) == 0)
        return rval_make_boolean(TRUE);
    return rval_make_boolean(FALSE);
}
