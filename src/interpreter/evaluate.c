#include <stdlib.h>

#include "evaluate.h"
#include "expr.h"
#include "rval.h"
#include "environment.h"
#include "funcs.h"

static Rval* evaluate_expression(Expr* expr);

static Rval* evaluate_literal(Expr* literal) {
    return make_rval_literal(literal->value);
}

static Rval* evaluate_call(Expr* call) {
    unsigned i;
    Expr* expr_list;
    Rval* ret;
    Rval* args = malloc(call->call.nargs * sizeof(struct Rval));
    expr_list = call->call.expr_list;
    for(i = 0; i < call->call.nargs; i++)
        args[i] = *(evaluate_expression(expr_list + i));

    return func_call(call->call.name, args);
}

static Rval* evaluate_unary(Expr* unary) {
    Rval* right = evaluate_expression(unary->unop.expr);
    if(right->type != RLITERAL) {
        // ERR cant negate matrix
        return NULL;
    }


    /* must be a double */
    switch(unary->unop.op) {
        case NEG:
            right->value.literal *= -1;
            return right;
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
    left = evaluate_expression(expr->binop.left);
    right = evaluate_expression(expr->binop.right);
 
    switch(expr->unop.op) {
        case ADD:
            if(!is_valid_operation(left, right)) {
                // ERR
                return NULL;
            }

            if(left->type == RLITERAL) {
                left->value.literal += right->value.literal;
            } else {
                if((m = matrix_add(left->value.matrix, right->value.matrix)) == NULL) {
                    // ERR
                    return NULL;
                }
                left->value.matrix = m;
            }
        case SUB:
            if(!is_valid_operation(left, right)) {
                // ERR
                return NULL;
            }

            if(left->type == RLITERAL) {
                left->value.literal -= right->value.literal;
            } else {
                if((m = matrix_subtract(left->value.matrix, right->value.matrix)) == NULL) {
                    // ERR
                    return NULL;
                }
                left->value.matrix = m;
            }
        case MULT:
            if(left->type == RLITERAL && right->type == RLITERAL) {
                left->value.literal *= right->value.literal;
            } else if(left->type == RLITERAL && right->type == RMATRIX) {
                if((m = matrix_multiply_constant(right->value.matrix, left->value.literal)) == NULL) {
                    // ERR
                    return NULL;
                }

                tmp = left;
                left = right;
                right = tmp;
                left->value.matrix = m;
            } else if(left->type == RMATRIX && right->type == RLITERAL) {
                if((m = matrix_multiply_constant(left->value.matrix, right->value.literal)) == NULL) {
                    // ERR
                    return NULL;
                }
                left->value.matrix = m;
            } else {
                if((m = matrix_multiply(left->value.matrix, right->value.matrix)) == NULL){
                    // ERR
                    return NULL;
                }
                left->value.matrix = m;
            }
            break;
        case DIV:
            if(left->type == RLITERAL) {
                left->value.literal /= right->value.literal;
            } else {
                // ERR 
                // cant divide matrices
                return NULL;
            }
            break;
        default:
            // ERR
            return NULL;
    }

    rval_destroy(right);
    return left;
}

static Rval* evaluate_grouping(Expr* grouping) {
    return evaluate_expression(grouping->grouping.expr);
}

static Matrix* evaluate_row(Expr* rexpr) {
    unsigned i;
    Matrix *m;
    Expr* e;
    Rval* val;
    m->values.literals = malloc(rexpr->matrix.ncols * sizeof(double));
    for(i = 0; i < rexpr->matrix.ncols; i++) {
        e = rexpr->matrix.expr_list + i;
        val = evaluate_expression(e);
        if(val->type == RMATRIX) {
            //err
            return NULL;
        }

        m->values.literals[i] = val->value.literal;
    }

    return m;
}

static Rval* evaluate_matrix(Expr* mexpr) {
    unsigned i;
    Rval *val, *ret;
    Matrix *row, *m;
    if(mexpr->matrix.nrows == 1) {
        m = evaluate_row(mexpr);
    } else {
        m->values.rows = malloc(mexpr->matrix.nrows * sizeof(struct Matrix));
        for(i = 0; i < mexpr->matrix.nrows; i++) {
            row = evaluate_row(mexpr->matrix.expr_list + i);
            m->values.rows[i] = *row;
        }
    }

    return make_rval_matrix(m);
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
    Rval* val = evaluate_expression(print->value.expr.expr);
    rval_print(val);
}

static void evaluate_var_statement(Stmt* var) {
    Rval* value = NULL;
    if(var->value.var.initializer != NULL)
        value = evaluate_expression(var->value.var.initializer);
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
        default:
            //err
            break;
    }
}
