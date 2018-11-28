#ifndef STATEMENT_H
#define STATEMENT_H

#include "scanner.h"
#include "expr.h"

typedef struct Stmt {
    enum {
        PRINT_S, EXPR_S, VAR_S
    } type;

    union {
        struct {
            char* name;
            Expr* initializer;
        } var;

        struct {
            Expr* expr;
        } expr;
    } value;
} Stmt;

Stmt* stmt_make_print(Expr* expr);
Stmt* stmt_make_expr(Expr* expr);
Stmt* stmt_make_var(Token* name, Expr* initializer);

#endif
