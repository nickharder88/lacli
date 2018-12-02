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
    tlist = token_list;
    index = 0;
}

static char tokens_empty(void) {
    return index >= tlist->count;
}

static Token* tokens_peek(unsigned count) {
    if(tokens_empty())
        return NULL;
    if(index + count >= tlist->count)
        return NULL;
    return tlist->arr[index + count];
}

static Token* tokens_advance(void) {
    Token* tkn;
    if(tokens_empty())
        return NULL;
    tkn = tlist->arr[index];
    index++;
    return tkn;
}

/*
 * STATEMENT
 */

static Stmt* expr_statement(void) {
    Expr* expr;
    if((expr = expression()) == NULL)
        return NULL;
    return stmt_make_expr(expr);
}

static Stmt* print_statement(void) {
    Expr* value;
    if((value = expression()) == NULL) {
        return NULL;
    }
    return stmt_make_print(value);
}

static Stmt* var_statement(Stmt* (*func)(char*, Expr*)) {
    Token *tkn, *name;
    Expr* initializer = NULL;

    if((name = tokens_advance()) == NULL || name->type != IDENTIFIER) {
        printf("Error: expected variable identifier\n");
        return NULL;
    }

    if((tkn = tokens_peek(0)) == NULL) {
        printf("Error: unexpected end of line\n");
        return NULL;
    }

    if(tkn->type != EQUAL) {
        printf("Error: expected =\n");
        return NULL;
    }

    tokens_advance();
    if((initializer = expression()) == NULL)
        return NULL;

    return func(name->literal.lexeme, initializer);
}

static Stmt* check_var_statement(Stmt* (*func)(char*, Expr*)) {
    Token* tkn;

    if((tkn = tokens_peek(1)) == NULL) {
        // example
        // var a = [[1,2],[1,2]]
        // a
        return NULL;
    }

    return var_statement(func);
}


/* 
 * EXPRESSIONS 
 */

static Expr* primary(void) {
    unsigned char ncol_checked = 0;
    unsigned size, nrows, ncols, i;
    Token* tkn;
    Expr *expr, **expr_list;

    if((tkn = tokens_peek(0)) == NULL) {
        printf("Error: unexpected end of line\n");
        return NULL;
    }

    if(tkn->type == IDENTIFIER) {
        tokens_advance();
        return expr_make_variable(tkn->literal.lexeme);
    }

    if(tkn->type == LEFT_PAREN) {
        tokens_advance();
        if((expr = expression()) == NULL)
            return NULL;

        if((tkn = tokens_peek(0)) == NULL) {
            printf("Error: unexpected end of line\n");
            expr_free(expr);
            return NULL;
        }

        if(tkn->type != RIGHT_PAREN) {
            printf("Error: expected )\n");
            expr_free(expr);
            return NULL;
        }
        tokens_advance();
        return expr_make_grouping(expr);
    }

    if(tkn->type == LEFT_BRACE) {
        tokens_advance();

        if((tkn = tokens_peek(0)) == NULL) {
            printf("Error: unexpected end of line\n");
            return NULL;
        }

        nrows = ncols = 0;
        size = MATRIXBASESIZE;
        expr_list = malloc(size * sizeof(struct Expr *));

        if(tkn->type == LEFT_BRACE) {
            // 2 dimensional matrix
            do {
                if(nrows >= size) {
                    size *= 2;
                    expr_list = realloc(expr_list, size * sizeof(struct Expr));
                }

                if((expr = primary()) == NULL) {
                    expr_free(expr);
                    for(i = 0; i < nrows; i++)
                        expr_free(expr_list[i]);
                    free(expr_list);
                }

                if(expr->type != MATRIX || expr->matrix.nrows != 1) {
                    expr_free(expr);
                    for(i = 0; i < nrows; i++)
                        expr_free(expr_list[i]);
                    free(expr_list);
                    printf("Error: matrix must be either one or two dimensions\n");
                    return NULL;
                }

                expr_list[nrows++] = expr;

                if(ncol_checked) {
                    if(expr->matrix.ncols != ncols) {
                        for(i = 0; i < nrows; i++)
                            expr_free(expr_list[i]);
                        free(expr_list);
                        printf("Error: matrix rows have different column lengths\n");
                        return NULL;
                    }
                } else
                    ncols = expr->matrix.ncols;

                if((tkn = tokens_peek(0)) == NULL) {
                    printf("Error: unexpected end of line\n");
                    for(i = 0; i < nrows; i++)
                        expr_free(expr_list[i]);
                    free(expr_list);
                    return NULL;
                }

                if(tkn->type == COMMA) {
                    tokens_advance();
                } else if(tkn->type == RIGHT_BRACE) {
                    tokens_advance();
                    return expr_make_matrix(expr_list, nrows, ncols);
                } else {
                    for(i = 0; i < nrows; i++)
                        expr_free(expr_list[i]);
                    free(expr_list);
                    printf("Error: invalid token\n");
                    return NULL;
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

                if((expr = expression()) == NULL) {
                    for(i = 0; i < ncols; i++)
                        expr_free(expr_list[i]);
                    free(expr_list);
                    return NULL;
                }

                expr_list[ncols++] = expr;

                if((tkn = tokens_peek(0)) == NULL) {
                    printf("Error: unexpected end of line\n");
                    for(i = 0; i < ncols; i++)
                        expr_free(expr_list[i]);
                    free(expr_list);
                    return NULL;
                }

                if(tkn->type == COMMA) {
                    tokens_advance();
                } else if(tkn->type == RIGHT_BRACE) {
                    tokens_advance();
                    return expr_make_matrix(expr_list, nrows, ncols);
                } else {
                    for(i = 0; i < ncols; i++)
                        expr_free(expr_list[i]);
                    free(expr_list);
                    printf("Error: invalid token\n");
                    return NULL;
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
    char* func_name;
    unsigned char nargs = 0, i;
    Token* tkn;
    Expr **expr_list, *expr;

    if((expr = primary()) == NULL)
        return NULL;

    if((tkn = tokens_peek(0)) == NULL || expr->type != VARIABLE)
        return expr;

    func_name = expr->identifier;

    if(tkn->type == LEFT_PAREN) {
        tokens_advance();

        if((tkn = tokens_peek(0)) == NULL) {
            printf("Error: unexpected end of line");
            expr_free(expr);
            return NULL;
        }

        if(tkn->type == RIGHT_PAREN) {
            // consume right parenthesis
            tokens_advance();
            return expr_make_call(expr->identifier, NULL, 0);
        }

        expr_list = malloc(MAXARGLIST * sizeof(struct Expr * ));

        while(nargs < MAXARGLIST) {
            if((expr = expression()) == NULL) {
                for(i = 0; i < nargs; i++)
                    expr_free(expr_list[i]);
                free(expr_list);
                return NULL;
            }

            expr_list[nargs++] = expr;

            if((tkn = tokens_peek(0)) == NULL) {
                printf("Error: unexpected end of line");
                for(i = 0; i < nargs; i++)
                    expr_free(expr_list[i]);
                free(expr_list);
                return NULL;
            }

            if(tkn->type != COMMA) {
                break;
            }
            tokens_advance();
        }

        if((tkn = tokens_peek(0)) == NULL || tkn->type != RIGHT_PAREN) {
            for(i = 0; i < nargs; i++)
                expr_free(expr_list[i]);
            free(expr_list);
            printf("Error: expected )\n");
            return NULL;
        }

        // consume RIGHT_PAREN
        tokens_advance();

        return expr_make_call(func_name, expr_list, nargs);
    }

    return expr;
}

static Expr* unary(void) {
    Token* tkn;
    Expr* un;

    if((tkn = tokens_peek(0)) == NULL) {
        printf("Error: unexpected end of line\n");
        return NULL;
    }

    if(tkn->type == MINUS) {
        tokens_advance();

        if((un = unary()) == NULL)
            return NULL;

        return expr_make_un_op(un, NEG);
    }

    return call();
}

static Expr* multiplication(void) {
    Token* tkn;
    Expr* left, *un;

    if((left = unary()) == NULL)
        return NULL;

    while((tkn = tokens_peek(0)) != NULL && (tkn->type == FSLASH || tkn->type == STAR)) {
        tokens_advance();
        if((un = unary()) == NULL) {
            expr_free(left);
            return NULL;
        }

        left = expr_make_bin_op(left, un, tkn->type == STAR ? MULT : DIV);
    }

    return left;
}

static Expr* addition(void) {
    Expr* left, *mult;
    Token* tkn;

    if((left = multiplication()) == NULL)
        return NULL;

    while((tkn = tokens_peek(0)) != NULL && (tkn->type == MINUS || tkn->type == PLUS)) {
        tokens_advance();

        if((mult = multiplication()) == NULL) {
            expr_free(left);
            return NULL;
        }

        left = expr_make_bin_op(left, mult, tkn->type == PLUS ? ADD : SUB);
    }

    return left;
}

static Expr* expression(void) {
    return addition();
}

static Stmt* declaration(void) {
    Token* tkn = tokens_peek(0);
    if(tkn == NULL) {
        return NULL;
    }

    if(tkn->type == VAR) {
        tokens_advance();
        return var_statement(stmt_make_var);
    } else if(tkn->type == IDENTIFIER) {
        return check_var_statement(stmt_make_assign);
    } else if(tkn->type == PRINT) {
        tokens_advance();
        return print_statement();
    }
    return expr_statement();
}

Stmt* parse(TokenList* tokenlist) {
    Token* tkn;
    Stmt* dec;
    tokens_reset(tokenlist);

    if((dec = declaration()) == NULL) {
        if(tlist->count > 0) {
            printf("Error: invalid expression\n");
        }
        return NULL;
    }

    if(tlist->count > index) {
        printf("Error: invalid expression\n");
        stmt_destroy(dec);
        return NULL;
    }

    return dec;
}
