#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "funcs.h"
#include "rref.h"
#include "aug.h"

Dict* func_create(void) {
    Dict* d = dict_create(func_destroy);
    dict_add(d, "rref", rref_handler);
    dict_add(d, "aug", aug_handler);
    return d;
}

/* expects the entire user input */
void* is_func(Dict* func_dict, char* func_name) {
    void* func;

    if((func = dict_get(func_dict, func_name)) == NULL) {
        return NULL;
    }

    return func;
}

void call_func(void* func_ptr, Dict* matrix_dict, char* line) {
    void (*func)(Dict* matrix_dict, char* line) = func_ptr;
    func(matrix_dict, line);
}

/* don't do anything to destroy functions in dictionary */
void func_destroy(void* data) {
    return;
}
