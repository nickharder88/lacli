#include "funcs.h"
#include "../dict.h"

#include "funcs/rref.h"
#include "funcs/aug.h"
#include "funcs/I.h"
#include "funcs/inverse.h"
#include "funcs/rank.h"
#include "funcs/det.h"
#include "funcs/trace.h"
#include "funcs/transpose.h"
#include "funcs/eval.h"
#include "funcs/evec.h"
#include "funcs/linind.h"
#include "funcs/span.h"
#include "funcs/null.h"

static Dict* func_dict;

void funcs_init(void) {
    func_dict = dict_create(NULL);

    /* Rval* (*func)(Rval* args, unsigned nargs) */
    dict_add(func_dict, "rref", rref_handler);
    dict_add(func_dict, "aug", aug_handler);
    dict_add(func_dict, "I", I_handler);
    dict_add(func_dict, "inverse", inverse_handler);
    dict_add(func_dict, "rank", rank_handler);
    dict_add(func_dict, "det", det_handler);
    dict_add(func_dict, "trace", trace_handler);
    dict_add(func_dict, "transpose", transpose_handler);
    dict_add(func_dict, "eval", eval_handler);
    dict_add(func_dict, "evec", evec_handler);
    dict_add(func_dict, "linind", linind_handler);
    dict_add(func_dict, "span", span_handler);
    dict_add(func_dict, "null", null_handler);
}

void funcs_destroy(void) {
    dict_destroy(func_dict);
}

/* expects the entire user input */
void* is_func(Dict* func_dict, char* func_name) {
    void* func;

    if((func = dict_get(func_dict, func_name)) == NULL) {
        return NULL;
    }

    return func;
}

Rval* func_call(char* name, Rval** args, unsigned nargs) {
    Rval* (*func)(Rval**, unsigned);

    if((func = dict_get(func_dict, name)) == NULL) {
        printf("Error: undefined function\n");
        return NULL;
    }

    return func(args, nargs);
}
