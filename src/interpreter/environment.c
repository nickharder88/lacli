#include "environment.h"
#include "../dict.h"

static Dict* var_dict;

static void var_destroy(void* rval) {
    Rval* val = (Rval*)rval;
    val->in_env = 0;
    rval_destroy(val);
}

void env_init(void) {
    var_dict = dict_create(var_destroy);
}

void env_destroy(void) {
    dict_destroy(var_dict);
}

void env_define(char* identifier, Rval* val) {
    if(identifier == NULL || val == NULL) {
        //err
    }

    if(dict_add(var_dict, identifier, val) == NULL) {
        printf("Error saving value\n");
        return;
    }

    val->in_env = 1;
}

Rval* env_get(char* identifier) {
    void* val;

    if((val = dict_get(var_dict, identifier)) == NULL) {
        //ERR
    }

    return (Rval*) val;
}
