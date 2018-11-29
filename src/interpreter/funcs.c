#include "funcs.h"

static Dict* func_dict;

void funcs_init(void) {
    func_dict = dict_create(NULL);
}

Rval* func_call(char* name, Rval* args) {
    //TODO
}
