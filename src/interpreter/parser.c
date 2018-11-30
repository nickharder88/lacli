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

static char tokens_empty(void) {
    return index >= tlist->count;
}

static Token* tokens_peek(void) {
    if(tokens_empty())
        return NULL;
    return tlist->arr + index;
}

static Token* tokens_advance(void) {
    Token* tkn;
    if(tokens_empty())
        return NULL;
    tkn = tlist->arr + index;
    index++;
    return tkn;
}

/*
 * STATEMENT
 */

static Stmt* expr_statement(void) {
    Expr* expr = expression();
    return stmt_make_expr(expr);
}

static Stmt* print_statement(void) {
    Expr* value = expression();
    return stmt_make_print(value);
}

static Stmt* var_statement(Stmt* (*func)(char*, Expr*)) {
    Token *tkn, *name;
    Expr* initializer = NULL;

    name = tokens_advance();
    if((tkn = tokens_peek()) != NULL && tkn->type == EQUAL) {
        tokens_advance();
        initializer = expression();
    }

    return func(name->literal.lexeme, initializer);
}

/* 
 * EXPRESSIONS 
 */

static Expr* primary(void) {
    unsigned char ncol_checked = 0;
    unsigned size, nrows, ncols;
    Token* tkn;
    Expr *expr, *expr_list;

    if((tkn = tokens_peek()) != NULL && tkn->type == IDENTIFIER) {
        tokens_advance();
        return expr_make_variable(tkn->literal.lexeme);
    }

    if((tkn = tokens_peek()) != NULL && tkn->type == LEFT_PAREN) {
        tokens_advance();
        expr = expression();
        if((tkn = tokens_peek()) != NULL && tkn->type != RIGHT_PAREN) {
            // ERR
        }
        tokens_advance();
        return expr_make_grouping(expr);
    }

    if(tkn->type == LEFT_BRACE) {
        tokens_advance();

        nrows = ncols = 0;
        size = MATRIXBASESIZE;
        expr_list = malloc(size * sizeof(struct Expr));

        if((tkn = tokens_peek()) != NULL && tkn->type == LEFT_BRACE) {
            // 2 dimensional matrix
            do {
                if(nrows >= size) {
                    size *= 2;
                    expr_list = realloc(expr_list, size * sizeof(struct Expr));
                }
                if((expr = primary())->type != MATRIX || expr->matrix.nrows != 1) {
                    //err
                    return NULL;
                }

                expr_list[nrows++] = *expr;

                if(ncol_checked) {
                    if(expr->matrix.ncols != ncols) {
                        //err
                        return NULL;
                    }
                } else
                    ncols = expr->matrix.ncols;

                if((tkn = tokens_peek()) != NULL && tkn->type == COMMA) {
                    tokens_advance();
                } else if(tkn->type == RIGHT_BRACE) {
                    tokens_advance();
                    return expr_make_matrix(expr_list, nrows, ncols);
                } else {
                    //err
                }
            } while(1);

        } else {
            // 1 dimensional matrix
            nrows = 1;
            do {
                if(ncols >= size) {
                    size *= 2;
                    expr_list = realloc(expr_list, size * sizeof(struct Expr));
                }
                expr_list[ncols++] = *(expression());

                if((tkn = tokens_peek()) != NULL && tkn->type == COMMA) {
                    tokens_advance();
                } else if(tkn->type == RIGHT_BRACE) {
                    tokens_advance();
                    // TODO
                    return expr_make_matrix(expr_list, nrows, ncols);
                } else {
                    //err
                }
            } while (1);
        }
    }

    if(tkn->type == NUMBER) {
        tokens_advance();
        return expr_make_literal(tkn->literal.number);
    }

    return NULL;
}

static Expr* call(void) {
    unsigned char nargs;
    Token* tkn;
    Expr *expr_list, *expr = primary();

    if((tkn = tokens_peek()) != NULL && tkn->type == LEFT_PAREN) {
        if(expr->type != VARIABLE) {
            //err
        }


        if((tkn = tokens_peek()) != NULL && tkn->type == RIGHT_PAREN) {
            // consume right parenthesis
            tokens_advance();
            return expr_make_call(expr->identifier, NULL, 0);
        }

        expr_list = malloc(MAXARGLIST * sizeof(struct Expr));

        for(nargs = 0; nargs < MAXARGLIST; nargs++) {
            expr_list[nargs] = *expression();

            if((tkn = tokens_peek()) != NULL && tkn->type != COMMA) {
                break;
            }
            tokens_advance();
        }

        if((tkn = tokens_peek()) != NULL && tkn->type != RIGHT_PAREN) {
            // ERR
        }

        return expr_make_call(expr->identifier, expr_list, nargs);
    }

    return expr;
}

static Expr* unary(void) {
    Token* tkn;
    if((tkn = tokens_peek()) != NULL && tkn->type == MINUS) {
        tokens_advance();
        return expr_make_un_op(unary(), NEG);
    }

    return call();
}

static Expr* multiplication(void) {
    Token* tkn;
    Expr* left = unary();

    while((tkn = tokens_peek()) != NULL && (tkn->type == FSLASH || tkn->type == STAR)) {
        tokens_advance();
        left = expr_make_bin_op(left, unary(), tkn->type == STAR ? MULT : DIV);
    }

    return left;
}

static Expr* addition(void) {
    Expr* left;
    Token* tkn;

    left = multiplication();
    while((tkn = tokens_peek()) != NULL && (tkn->type == MINUS || tkn->type == PLUS)) {
        tokens_advance();
        left = expr_make_bin_op(left, multiplication(), tkn->type == PLUS ? ADD : SUB);
    }
    return left;
}

static Expr* expression(void) {
    return addition();
}

static Stmt* declaration(void) {
    Token* tkn = tokens_peek();
    if(tkn == NULL) {
        return NULL;
    }

    if(tkn->type == VAR) {
        tokens_advance();
        return var_statement(stmt_make_var);
    } else if(tkn->type == IDENTIFIER) {
        return var_statement(stmt_make_assign);
    } else if(tkn->type == PRINT) {
        tokens_advance();
        return print_statement();
    }

    return expr_statement();
}

Stmt* parse(TokenList* tokenlist) {
    tokens_reset(tokenlist);
    return declaration();
}
