#include <stdlib.h>

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

Stmt* stmt_make_var(Token* name, Expr* initializer) {
    Stmt* stmt = malloc(sizeof(struct Stmt));
    stmt->type = VAR_S;
    stmt->value.var.initializer = initializer;
    stmt->value.var.name = name->literal.lexeme;
    return stmt;
}
