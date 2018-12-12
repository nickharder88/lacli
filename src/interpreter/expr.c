#include <stdlib.h>
#include <string.h>

#include "../matrix.h"
#include "expr.h"

Expr* expr_make_bin_op(Expr* left, Expr* right, Operator op) {
    Expr* bop = malloc(sizeof(struct Expr));
    bop->binop.left = left;
    bop->binop.right = right;
    bop->binop.op = op;
    bop->type = BINOP;
    return bop;
}

Expr* expr_make_un_op(Expr* expr, Operator op) {
    Expr* uop = malloc(sizeof(struct Expr));
    uop->unop.expr = expr;
    uop->unop.op = op;
    uop->type = UNOP;
    return uop;
}

Expr* expr_make_call(char* name, Expr** expr_list, unsigned nargs) {
    Expr* cexp = malloc(sizeof(struct Expr));
    cexp->call.name = strdup(name);
    cexp->call.expr_list = expr_list;
    cexp->call.nargs = nargs;
    cexp->type = CALL;
    return cexp;
}

Expr* expr_make_grouping(Expr* expr) {
    Expr* group = malloc(sizeof(struct Expr));
    group->grouping.expr = expr;
    group->type = GROUPING;
    return group;
}

Expr* expr_make_literal(double val) {
    Expr* value = malloc(sizeof(struct Expr));
    value->value = val;
    value->type = LITERAL;
    return value;
}

Expr* expr_make_variable(char* val) {
    Expr* value = malloc(sizeof(struct Expr));
    value->identifier = strdup(val);
    value->type = VARIABLE;
    return value;
}

Expr* expr_make_matrix(Expr** expr_list, unsigned nrows, unsigned ncols) {
    Expr* mexpr = malloc(sizeof(struct Expr));
    mexpr->matrix.expr_list = expr_list;
    mexpr->matrix.nrows = nrows;
    mexpr->matrix.ncols = ncols;
    mexpr->type = MATRIX;
    return mexpr;
}

void expr_destroy(Expr* expr) {
    unsigned i;

    switch(expr->type) {
        case BINOP:
            expr_destroy(expr->binop.left);
            expr_destroy(expr->binop.right);
            break;
        case UNOP:
            free(expr->unop.expr);
            break;
        case CALL:
            for(i = 0; i < expr->call.nargs; i++)
                expr_destroy(expr->call.expr_list[i]);
            free(expr->call.expr_list);
            free(expr->call.name);
            break;
        case GROUPING:
            expr_destroy(expr->grouping.expr);
            break;
        case LITERAL:
            break;
        case MATRIX:
            if(expr->matrix.nrows > 1) {
                for(i = 0; i < expr->matrix.nrows; i++)
                    expr_destroy(expr->matrix.expr_list[i]);
                free(expr->matrix.expr_list);
            } else {
                for(i = 0; i < expr->matrix.ncols; i++)
                    expr_destroy(expr->matrix.expr_list[i]);
                free(expr->matrix.expr_list);
            }
            break;
        case VARIABLE:
            if(expr->identifier != NULL)
                free(expr->identifier);
            break;
    }
    free(expr);
}
