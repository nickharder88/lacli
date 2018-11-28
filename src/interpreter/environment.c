#include "environment.h"
#include "../dict.h"

static Dict* var_dict;

static void var_destroy(void* rval) {
    Rval* val = (Rval*)rval;
    rval_destroy(val);
}

void env_init(void) {
    var_dict = dict_create(var_destroy);
}

void env_destroy(void) {
    dict_clear(var_dict);
}

void env_define(char* identifier, Rval* val) {
    if(identifier == NULL || val == NULL) {
        //err
    }

    dict_add(var_dict, identifier, val);
}

Rval* env_get(char* identifier) {
    void* val;

    if((val = dict_get(var_dict, identifier)) == NULL) {
        //ERR
    }

    return (Rval*) val;
}
