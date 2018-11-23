#include <stdlib.h>

#include "parser.h"
#include "../matrix.h"

#define MATRIXBASESIZE 2

/* Method declarations */
static Expr* expression(void);

static TokenList* tlist;
static unsigned index;

static void tokens_reset(TokenList* token_list) {
    if(tlist != NULL)
        token_destroy(tlist);
    tlist = token_list;
    index = 0;
}

static Token* tokens_peek(void) {
    return tlist->arr + index;
}

static char tokens_empty(void) {
    return index >= tlist->count;
}

static char tokens_check_type(TokenType type) {
    return !tokens_empty() && tokens_peek()->type == type;
}

static Token* tokens_advance(void) {
    if(!tokens_empty())
        index++;
}

static Token* tokens_previous(void) {
    if(index <= 0)
        return NULL;
    return tlist->arr + (index - 1);
}

static Expr* primary(void) {
    Matrix* m;
    unsigned size, count;
    Token* operator;
    Expr *expr, *expr_list;

    if((operator = tokens_peek())->type == LEFT_PAREN) {
        expr = expression();
        if(tokens_peek()->type != RIGHT_PAREN) {
            // ERR
        }
        tokens_advance();
        return make_grouping(expr);
    }

    if(operator->type == LEFT_BRACE) {
        tokens_advance();

        count = 0;
        size = MATRIXBASESIZE;
        expr_list = malloc(size * sizeof(struct Expr));

        do {
            if(count >= size) {
                size *= 2;
                expr_list = realloc(expr_list, size * sizeof(struct Expr));
            }
            expr_list[count++] = *(expression());
        } while((operator = tokens_peek())->type == COMMA);

        if(tokens_peek()->type != RIGHT_BRACE) {
            //ERR
        }
        tokens_advance();
        return make_matrix(expr_list, count);
    }

    if(operator->type == NUMBER) {
        tokens_advance();
        return make_literal_expr(operator->literal.number);
    }

    return NULL;
}

static Expr* unary(void) {
    Expr* right;
    Token* operator;
    if((operator = tokens_peek())->type == MINUS) {
        tokens_advance();
        return make_un_op(unary(), operator->type);
    }

    return primary();
}

static Expr* multiplication(void) {
    Token* operator;
    Expr* left = unary();

    while((operator = tokens_peek())->type == FSLASH || operator->type == STAR) {
        tokens_advance();
        left = make_bin_op(left, unary(), operator->type);
    }

    return left;
}

static Expr* addition(void) {
    Expr* left;
    Token* operator;

    left = multiplication();
    while((operator = tokens_peek())->type == MINUS || operator->type == PLUS) {
        tokens_advance();
        left = make_bin_op(left, multiplication(), operator->type);
    }
    return left;
}

static Expr* expression(void) {
    return addition();
}


Expr* parse(TokenList* tokenlist) {
    tokens_reset(tokenlist);
    return expression();
}
