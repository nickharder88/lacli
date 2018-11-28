#include <stdlib.h>

#include "parser.h"
#include "../matrix.h"

#define MATRIXBASESIZE 2
#define MAXARGLIST 4

static Expr* expression(void);

/*
 * TOKEN HELPER METHODS
 */

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
    Token* tkn;
    if(tokens_empty())
        return NULL;
    tkn = tlist->arr + index;
    index++;
    return tkn;
}

static Token* tokens_previous(void) {
    if(index <= 0)
        return NULL;
    return tlist->arr + (index - 1);
}

/*
 * STATEMENT
 */

static Stmt* expr_statment(void) {
    Expr* expr = expression();
    return stmt_make_expr(expr);
}

static Stmt* print_statement(void) {
    Expr* value = expression();
    return stmt_make_print(value);
}

static Stmt* statement(void) {
    Token* tkn;
    if((tkn = tokens_peek())->type == PRINT) {
        tokens_advance();
        return print_statement();
    }
}

static Stmt* var_declaration(void) {
    Token *tkn, *name;
    Expr* initializer = NULL;

    name = tokens_advance();
    if((tkn = tokens_peek())->type == EQUAL) {
        tokens_advance();
        initializer = expression();
    }

    return stmt_make_var(name, initializer);
}

static Stmt* declaration(void) {
    Token* tkn;

    if((tkn = tokens_peek())->type == VAR) {
        tokens_advance();
        return var_declaration();
    }
    return statement();
}

/* 
 * EXPRESSIONS 
 */

static Expr* primary(void) {
    Matrix* m;
    unsigned size, count;
    Token* tkn;
    Expr *expr, *expr_list;

    if((tkn = tokens_peek())->type == IDENTIFIER) {
        tokens_advance();
        return expr_make_variable(tkn->literal.lexeme);
    }

    if((tkn = tokens_peek())->type == LEFT_PAREN) {
        tokens_advance();
        expr = expression();
        if(tokens_peek()->type != RIGHT_PAREN) {
            // ERR
        }
        tokens_advance();
        return expr_make_grouping(expr);
    }

    if(tkn->type == LEFT_BRACE) {
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
        } while((tkn = tokens_peek())->type == COMMA);

        if(tokens_peek()->type != RIGHT_BRACE) {
            //ERR
        }
        tokens_advance();
        return expr_make_matrix(expr_list, count);
    }

    if(tkn->type == NUMBER) {
        tokens_advance();
        return expr_make_literal(tkn->literal.number);
    }

    return NULL;
}

static Expr* get_arg_list(void) {
    unsigned char i;
    Expr* exprlist = malloc(MAXARGLIST * sizeof(struct Expr));

    if(tokens_peek()->type == RIGHT_PAREN) {
        // consume right parenthesis
        tokens_advance();
        return NULL;
    }

    for(i = 0; i < MAXARGLIST; i++) {
        exprlist[i] = *expression();

        if(tokens_peek()->type != COMMA) {
            break;
        }
        tokens_advance();
    }

    if(tokens_peek()->type != RIGHT_PAREN) {
        // ERR
    }

    return exprlist;
}

static Expr* call(void) {
    Token* tkn;
    Expr *expr_list, *expr = primary();

    if((tkn = tokens_peek())->type == LEFT_PAREN) {
        if(expr->type != IDENTIFIER) {
            //err
        }

        expr_list = get_arg_list();

        return expr_make_call(expr_list, expr->identifier);
    }

    return expr;
}

static Expr* unary(void) {
    Expr* right;
    Token* tkn;
    if((tkn = tokens_peek())->type == NEG) {
        tokens_advance();
        return expr_make_un_op(unary(), NEG);
    }

    return call();
}

static Expr* multiplication(void) {
    Token* tkn;
    Expr* left = unary();

    while((tkn = tokens_peek())->type == FSLASH || tkn->type == STAR) {
        tokens_advance();
        left = expr_make_bin_op(left, unary(), tkn->type);
    }

    return left;
}

static Expr* addition(void) {
    Expr* left;
    Token* tkn;

    left = multiplication();
    while((tkn = tokens_peek())->type == MINUS || tkn->type == PLUS) {
        tokens_advance();
        left = expr_make_bin_op(left, multiplication(), tkn->type);
    }
    return left;
}

static Expr* expression(void) {
    return addition();
}

Stmt* parse(TokenList* tokenlist) {
    tokens_reset(tokenlist);
    return declaration();
}
