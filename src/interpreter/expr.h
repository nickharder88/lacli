#ifndef EXPR_H
#define EXPR_H

#include "../matrix.h"

typedef enum Operator {
    ADD, SUB, MULT, DIV, NEG
} Operator;

typedef struct Expr {
    enum {
        BINOP, UNOP, CALL, GROUPING, LITERAL, MATRIX, VARIABLE
    } type;

    union {
        double value;
        char* identifier;

        struct {
            struct Expr* expr_list;
            unsigned length;
        } matrix;

        struct {
            struct Expr* left;
            struct Expr* right;
            Operator op;
        } binop;

        struct {
            struct Expr* expr;
            Operator op;
        } unop;

        struct {
            char* name;
            struct Expr* expr_list;
        } call;

        struct {
            struct Expr* expr;
        } grouping;
    };
} Expr;

Expr* expr_make_bin_op(Expr* left, Expr* right, Operator op);
Expr* expr_make_un_op(Expr* expr, Operator op);
Expr* expr_make_call(Expr* expr, char* name);
Expr* expr_make_grouping(Expr* expr);
Expr* expr_make_literal(double val);
Expr* expr_make_variable(char* val);
Expr* expr_make_matrix(Expr* expr_list, unsigned length);
void expr_free(Expr* expr);

#endif
