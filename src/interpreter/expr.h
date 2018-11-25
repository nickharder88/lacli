#include "../matrix.h"

typedef enum Operator {
    ADD, SUB, MULT, DIV, NEG
} Operator;

typedef struct Expr {
    enum {
        BINOP, UNOP, CALL, GROUPING, LITERAL, MATRIX
    } tag;

    union {
        double value;

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
            struct Expr* expr;
        } call;

        struct {
            struct Expr* expr;
        } grouping;
    };
} Expr;

Expr* make_bin_op(Expr* left, Expr* right, Operator op);
Expr* make_un_op(Expr* expr, Operator op);
Expr* make_call(Expr* expr, char* name);
Expr* make_grouping(Expr* expr);
Expr* make_literal_expr(double val);
Expr* make_matrix(Expr* expr_list, unsigned length);
void free_expr(Expr* expr);
