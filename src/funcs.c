#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "funcs.h"
#include "defs.h"
#include "rref.h"

Dict* func_create(void) {
    char* name;

    Dict* d = dict_create(func_destroy);
    dict_add(d, "rref", rref_handler);
}

/* expects the entire user input */
void* is_func(Dict* func_dict, char* line) {
    unsigned i;
    void* func;
    char func_name[MAXIDENTIFIER];

    for(i = 0; i < MAXIDENTIFIER - 1 && isalpha(*line); i++)
        func_name[i] = *line++;
    func_name[i] = '\0';

    if(*line != '(') {
        free(func_name);
        return NULL;
    }

    if((func = dict_get(func_dict, func_name)) == NULL) {
        return NULL;
    }

    return func;
}

void call_func(void* func_ptr, char* line) {
    void (*func)(char* line) = func_ptr;
    func(line);
}

/* don't do anything to destroy functions in dictionary */
void func_destroy(void* data) {
    return;
}
