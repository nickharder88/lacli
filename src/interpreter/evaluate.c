#include <stdlib.h>

#include "evaluate.h"
#include "expr.h"
#include "rval.h"

static void rval_free(Rval* rval) {
    if(rval->type == RMATRIX)
        matrix_destroy(rval->value.matrix);
    free(rval);
}

static Rval* make_rval_literal(double val) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = LITERAL;
    rval->value.literal = val;
    return rval;
}

static Rval* make_rval_matrix(Matrix* m) {
    Rval* rval = malloc(sizeof(struct Rval));
    rval->type = MATRIX;
    rval->value.matrix = m;
    return rval;
}


static Rval* evaluate_literal(Expr* literal) {
    return make_rval_literal(literal->value);
}

static Rval* evaluate_expression(Expr* expression) {
    // TODO
}

static Rval* evaluate_call(Expr* call) {

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

    rval_free(right);
    return left;
}

static Rval* evaluate_grouping(Expr* grouping) {
    return evaluate_expression(grouping->grouping.expr);
}

static Matrix* evaluate_matrix(Expr* mexpr) {
    Matrix* m;

    // TODO

    return m;
}

/*
 * STATEMENT
 */

static void evaluate_expr_statement(Stmt* expr) {
    evaluate_expression(expr->expr);
}

static void evaluate_print_statement(Stmt* print) {
    Rval* val = evaluate_expression(print->expr);
    rval_print(val);
}

static void evaluate_var_statement(Stmt* var) {
    if(var->initializer == NULL) {
    }
}

/*
 * INTERFACE
 */

void evaluate(Stmt* statement) {

}
