#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "funcs.h"
#include "rref.h"
#include "aug.h"
#include "I.h"
#include "inverse.h"
#include "rank.h"
#include "det.h"
#include "trace.h"
#include "transpose.h"
#include "eval.h"

Dict* func_create(void) {
    Dict* d = dict_create(func_destroy);
    dict_add(d, "rref", rref_handler);
    dict_add(d, "aug", aug_handler);
    dict_add(d, "I", I_handler);
    dict_add(d, "inverse", inverse_handler);
    dict_add(d, "rank", rank_handler);
    dict_add(d, "det", det_handler);
    dict_add(d, "trace", trace_handler);
    dict_add(d, "transpose", transpose_handler);
    dict_add(d, "eval", eval_handler);
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

Matrix** parse_args(Dict* matrix_dict, char* line, unsigned nmatrices, char** identifier) {
    unsigned i;
    Matrix *m;
    Matrix** marr = malloc(nmatrices * sizeof(Matrix*));

    if(*line++ != '(') {
       free(marr);
       return NULL;
    }

    for(i = 0; i < nmatrices; i++) {

        /* skip whitespace */
        while(*line == ' ' || *line == '\t')
            line++;

        /* could not get matrix */
        if((m = try_get_matrix(matrix_dict, &line)) == NULL) {
            free(marr);
            return NULL;
        }

        /* skip whitespace */
        while(*line == ' ' || *line == '\t')
            line++;

        if(i < nmatrices - 1) {
            if(*line != ',') {
                free(marr);
                return NULL;
            }
            line++;
        }
        
        marr[i] = m;
    }

    /* skip whitespace */
    while(*line == ' ' || *line == '\t')
        line++;

    if(*line == ')') {
        if(identifier != NULL)
            *identifier = NULL;
        return marr;
    } else if(*line != ',') {
        free(marr);
        return NULL;
    } else if(identifier == NULL) {
        free(marr);
        return NULL;
    }

    /* skip , */
    line++;

    /* skip whitespace */
    while(*line == ' ' || *line == '\t')
        line++;

    *identifier = malloc(MAXIDENTIFIER * sizeof(char));
    for(i = 0; i < MAXIDENTIFIER - 1 && isalnum(*line); i++)
        (*identifier)[i] = *line++;
    (*identifier)[i] = '\0';

    /* skip whitespace after second arg */
    while(*line == ' ' || *line == '\t')
        line++;

    /* error */
    if(*line != ')') {
        free(identifier);
        free(marr);
        return NULL;
    }

    return marr;
}
