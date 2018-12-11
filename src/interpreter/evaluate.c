#include <stdlib.h>

#include "evaluate.h"
#include "expr.h"
#include "rval.h"
#include "environment.h"
#include "funcs.h"

#include "../matrix.h"

static Rval* evaluate_expression(Expr* expr);

static Rval* evaluate_literal(Expr* literal) {
    return rval_make_literal(literal->value);
}

static Rval* evaluate_call(Expr* call) {
    unsigned i, j;
    Expr **expr_list, *expr;
    Rval *val, **args = malloc(call->call.nargs * sizeof(struct Rval *));

    expr_list = call->call.expr_list;
    for(i = 0; i < call->call.nargs; i++) {

        if((val = evaluate_expression(expr_list[i])) == NULL) {
            for(j = 0; j < i; j++) {
                rval_destroy(args[j]);
            }
            free(args);
            return NULL;
        }

        args[i] = val;
    }

    return func_call(call->call.name, args, call->call.nargs);
}

static Rval* evaluate_unary(Expr* unary) {
    Rval* right;
    if((right = evaluate_expression(unary->unop.expr)) == NULL) {
        return NULL;
    }

    if(right->type != RLITERAL) {
        rval_destroy(right);
        printf("Error: cannot negate a matrix\n");
        return NULL;
    }

    /* must be a double */
    switch(unary->unop.op) {
        case NEG:
            right->value.literal *= -1;
            return right;
        default:
            //err
            break;
    }

    return NULL;
}

static char is_valid_operation(Rval* left, Rval* right) {
    return (left->type == RLITERAL && right->type == RLITERAL) ||
        (left->type == RMATRIX && right->type == RMATRIX);
}

static Rval* evaluate_binary(Expr* expr) {
    Matrix* m;
    Rval *right, *left, *tmp;

    if((left = evaluate_expression(expr->binop.left)) == NULL)
        return NULL;

    if((right = evaluate_expression(expr->binop.right)) == NULL) {
        rval_destroy(left);
        return NULL;
    }
 
    switch(expr->binop.op) {
        case ADD:
            if(!is_valid_operation(left, right)) {
                printf("Error: invalid addition\n");
                rval_destroy(left);
                rval_destroy(right);
                return NULL;
            }

            if(left->type == RLITERAL) {
                left->value.literal += right->value.literal;
            } else {
                if((m = matrix_add(left->value.matrix, right->value.matrix)) == NULL) {
                    rval_destroy(left);
                    rval_destroy(right);
                    return NULL;
                }

                return rval_make_matrix(m);
            }
            break;
        case SUB:
            if(!is_valid_operation(left, right)) {
                printf("Error: invalid substitution\n");
                rval_destroy(left);
                rval_destroy(right);
                return NULL;
            }

            if(left->type == RLITERAL) {
                left->value.literal -= right->value.literal;
            } else {
                if((m = matrix_subtract(left->value.matrix, right->value.matrix)) == NULL) {
                    rval_destroy(left);
                    rval_destroy(right);
                    return NULL;
                }
                return rval_make_matrix(m);
            }
            break;
        case MULT:
            if(left->type == RLITERAL && right->type == RLITERAL) {
                left->value.literal *= right->value.literal;
            } else if(left->type == RLITERAL && right->type == RMATRIX) {
                if((m = matrix_multiply_constant(right->value.matrix, left->value.literal)) == NULL) {
                    rval_destroy(left);
                    rval_destroy(right);
                    return NULL;
                }

                tmp = left;
                left = right;
                right = tmp;
                return rval_make_matrix(m);
            } else if(left->type == RMATRIX && right->type == RLITERAL) {
                if((m = matrix_multiply_constant(left->value.matrix, right->value.literal)) == NULL) {
                    rval_destroy(left);
                    rval_destroy(right);
                    return NULL;
                }
                return rval_make_matrix(m);
            } else {
                if((m = matrix_multiply(left->value.matrix, right->value.matrix)) == NULL){
                    rval_destroy(left);
                    rval_destroy(right);
                    return NULL;
                }
                return rval_make_matrix(m);
            }
            break;
        case DIV:
            if(left->type == RLITERAL) {
                left->value.literal /= right->value.literal;
            } else {
                printf("Error: cannot divide matrices\n");
                rval_destroy(left);
                rval_destroy(right);
                return NULL;
            }
            break;
        default:
            // ERR
            return NULL;
    }

    return left;
}

static Rval* evaluate_grouping(Expr* grouping) {
    return evaluate_expression(grouping->grouping.expr);
}

static Matrix* evaluate_row(Expr* rexpr) {
    unsigned i;
    Expr* e;
    Rval* val;
    Matrix *m = matrix_create_dim(rexpr->matrix.nrows, rexpr->matrix.ncols);
    for(i = 0; i < rexpr->matrix.ncols; i++) {
        e = rexpr->matrix.expr_list[i];

        if((val = evaluate_expression(e)) == NULL) {
            matrix_destroy(m);
            return NULL;
        }

        if(val->type == RMATRIX) {
            matrix_destroy(m);
            printf("Error: evaluating matrix with high dimension\n");
            return NULL;
        }

        m->values.literals[i] = val->value.literal;
        rval_destroy(val);
    }

    return m;
}

static Rval* evaluate_matrix(Expr* mexpr) {
    unsigned i;
    Matrix *row, *m;
    if(mexpr->matrix.nrows == 1) {
        if((m = evaluate_row(mexpr)) == NULL)
            return NULL;
    } else {
        m = matrix_create_dim_uninitialized(mexpr->matrix.nrows);
        for(i = 0; i < mexpr->matrix.nrows; i++) {
            if((row = evaluate_row(mexpr->matrix.expr_list[i])) == NULL) {
                matrix_destroy(m);
                return NULL;
            }
            m->values.rows[i] = row;
        }
    }

    return rval_make_matrix(m);
}

static Rval* evaluate_expression(Expr* expr) {
    Rval* val;

    switch(expr->type) {
        case BINOP:
            val = evaluate_binary(expr);
            break;
        case UNOP:
            val = evaluate_unary(expr);
            break;
        case CALL:
            val = evaluate_call(expr);
            break;
        case GROUPING:
            val = evaluate_grouping(expr);
            break;
        case LITERAL:
            val = evaluate_literal(expr);
            break;
        case MATRIX:
            val = evaluate_matrix(expr);
            break;
        case VARIABLE:
            val = env_get(expr->identifier);
            break;
        default:
            //err
            break;
    }

    return val;
}

/*
 * STATEMENT
 */

static void evaluate_expr_statement(Stmt* expr) {
    evaluate_expression(expr->value.expr.expr);
}

static void evaluate_print_statement(Stmt* print) {
    Rval* val;
    if((val = evaluate_expression(print->value.expr.expr)) == NULL) {
        printf("Error: cannot print this expression\n");
        return;
    }
    rval_print(val);
    /* rval_destroy(val); */
}

static void evaluate_assign_statement(Stmt* assign) {
    Rval* value = NULL;

    if(env_get(assign->value.var.name) == NULL)  {
        printf("Error: identifier %s not defined (define with var)\n", assign->value.var.name);
        return;
    }

    if(assign->value.var.initializer == NULL)
        value = rval_make_nil();
    else
        if((value = evaluate_expression(assign->value.var.initializer)) == NULL)
            return;
    env_define(assign->value.var.name, value);
}

static void evaluate_var_statement(Stmt* var) {
    Rval* value = NULL;
    if(var->value.var.initializer == NULL)
        value = rval_make_nil();
    else
        if((value = evaluate_expression(var->value.var.initializer)) == NULL)
            return;
    env_define(var->value.var.name, value);
}

/*
 * INTERFACE
 */

void evaluate(Stmt* statement) {
    switch(statement->type) {
        case PRINT_S:
            evaluate_print_statement(statement);
            break;
        case EXPR_S:
            evaluate_expr_statement(statement);
            break;
        case VAR_S:
            evaluate_var_statement(statement);
            break;
        case ASSIGN_S:
            evaluate_assign_statement(statement);
            break;
        default:
            //err
            break;
    }
}
