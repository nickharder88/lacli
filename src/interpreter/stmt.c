#include <stdlib.h>
#include <string.h>

#include "stmt.h"

Stmt* stmt_make_print(Expr* expr) {
    Stmt* stmt = malloc(sizeof(struct Stmt));
    stmt->type = PRINT_S;
    stmt->value.expr.expr = expr;
    return stmt;
}

Stmt* stmt_make_expr(Expr* expr) {
    Stmt* stmt = malloc(sizeof(struct Stmt));
    stmt->type = EXPR_S;
    stmt->value.expr.expr = expr;
    return stmt;
}

Stmt* stmt_make_var(char* name, Expr* initializer) {
    Stmt* stmt = malloc(sizeof(struct Stmt));
    stmt->type = VAR_S;
    stmt->value.var.initializer = initializer;
    stmt->value.var.name = strdup(name);
    return stmt;
}

Stmt* stmt_make_assign(char* name, Expr* initializer) {
    Stmt* stmt = malloc(sizeof(struct Stmt));
    stmt->type = ASSIGN_S;
    stmt->value.var.initializer = initializer;
    stmt->value.var.name = strdup(name);
    return stmt;
}

void stmt_destroy(Stmt* stmt) {
    switch(stmt->type) {
        case PRINT_S:
            expr_destroy(stmt->value.expr.expr);
            break;
        case EXPR_S:
            expr_destroy(stmt->value.expr.expr);
            break;
        case VAR_S:
            free(stmt->value.var.name);
            expr_destroy(stmt->value.var.initializer);
            break;
        case ASSIGN_S:
            free(stmt->value.var.name);
            expr_destroy(stmt->value.var.initializer);
            break;
        default:
            break;
    }

    free(stmt);
}
