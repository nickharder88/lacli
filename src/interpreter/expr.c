#include <stdlib.h>

#include "../matrix.h"
#include "expr.h"

Expr* make_bin_op(Expr* left, Expr* right, Operator op) {
    Expr* bop = malloc(sizeof(struct Expr));
    bop->binop.left = left;
    bop->binop.right = right;
    bop->binop.op = op;
    bop->tag = BINOP;
    return bop;
}

Expr* make_un_op(Expr* expr, Operator op) {
    Expr* uop = malloc(sizeof(struct Expr));
    uop->unop.expr = expr;
    uop->unop.op = op;
    uop->tag = UNOP;
    return uop;
}

Expr* make_call(Expr* expr, char* name) {
    Expr* cexp = malloc(sizeof(struct Expr));
    cexp->call.name = name;
    cexp->call.expr = expr;
    cexp->tag = CALL;
    return cexp;
}

Expr* make_grouping(Expr* expr) {
    Expr* group = malloc(sizeof(struct Expr));
    group->grouping.expr = expr;
    group->tag = GROUPING;
    return group;
}

Expr* make_literal_expr(double val) {
    Expr* value = malloc(sizeof(struct Expr));
    value->value = val;
    value->tag = LITERAL;
    return value;
}

Expr* make_matrix(Expr* expr_list, unsigned length) {
    Expr* mexpr = malloc(sizeof(struct Expr));
    mexpr->matrix.expr_list = expr_list;
    mexpr->matrix.length = length;
    mexpr->tag = MATRIX;
    return mexpr;
}
