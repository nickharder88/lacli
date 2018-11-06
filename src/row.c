#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "row.h"

void row_destroy(Row *r) {
    free(r->vals);
}

Row* row_copy(Row* r) {
    unsigned i;

    Row* copy = malloc(sizeof(struct Row));
    copy->len = r->len;
    copy->vals = malloc(sizeof(double) * copy->len);
    copy->pivot = r->pivot;

    for(i = 0; i < copy->len; i++)
        copy->vals[i] = r->vals[i];

    return copy;
}

unsigned check_row_size(Row* ptr, unsigned size) {
    double* vals;

    if(ptr->len < size) {
        return size;
    }

    size *= 2;
    if((vals = realloc(ptr->vals, size * sizeof(double))) == NULL) {
        printf("Error: could not allocate memory. Try again.\n");
        row_destroy(ptr);
        free(ptr);
        return 0;
    }
    ptr->vals = vals;

    return size;
}

Row* row_parse(char** line) {
    char c, *ptr = *line, sign = 1;
    double dig = 0, *vals;
    unsigned size = 1;

    Row* row = malloc(sizeof(Row));
    row->len = 0;
    row->vals = malloc(size * sizeof(double));

    if((c = *ptr++) != '[') {
        printf("Error: Row must start with a [\n");
        row_destroy(row);
        free(row);
        return NULL;
    }

    while((c = *ptr++) != '\0') {
        if(c == ']') {
            if((size = check_row_size(row, size)) == 0) {
                return NULL;
            }
            dig *= sign;
            row->vals[row->len++] = dig;
            break;
        } else if(c == ',') {
            if((size = check_row_size(row, size)) == 0) {
                return NULL;
            }
            dig *= sign;
            row->vals[row->len++] = dig;

            dig = 0;
            sign = 1;
        } else if(isdigit(c)) {
            dig = dig * 10 + c - '0';
        } else if(c == '-') {
            if(dig) {
                printf("Error: Invalid negative sign in matrix\n");
                row_destroy(row);
                free(row);
            }
            sign = -1;
        } else if(!isspace(c)) {
            printf("Error: Invalid character in matrix defintion : %c\n", c);
            row_destroy(row);
            free(row);
            return NULL;
        }
    }

    if(c == '\0') {
        row_destroy(row);
        free(row);
        return NULL;
    }

    if((vals = realloc(row->vals, row->len * sizeof(double))) == NULL) {
        printf("Error: could not allocate memory. Try again.\n");
        row_destroy(row);
        free(row);
        return NULL;
    }
    row->vals = vals;
    *line = ptr;
    return row;
}

void row_multiply(Row* a, Row* b, double scalar) {
    unsigned i;
    for(i = 0; i < a->len; i++)
        a->vals[i] += b->vals[i] * scalar;
}


void row_scale(Row* a, double scalar) {
    unsigned i;
    for(i = 0; i < a->len; i++)
        a->vals[i] *= scalar;
}
