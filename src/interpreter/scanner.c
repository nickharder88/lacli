#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "../defs.h"
#include "scanner.h"
#include "error.h"
#include "../dict.h"

#define VARSTR "var"
#define PRINTSTR "print"
#define QUITSTR "quit"

#define DEFAULTSIZE 8

static char* ptr;
static unsigned ptrlen;
static unsigned ptrindex;

static void scan_init(char* line, unsigned len) {
    ptr = line;
    ptrlen = len;
    ptrindex = 0;
}

static char scan_next() {
    if(ptrindex < ptrlen) {
        return ptr[ptrindex++];
    }
    return '\0';
}

static double scan_dig(char c) {
    char has_fraction;
    unsigned i, power = 1;
    double dig = c - '0';

    for(i = 0; i < MAXDIGIT - 1 
            && ptrindex < ptrlen 
            && (isdigit((c = ptr[ptrindex]))
                || c == '.'); 
                ptrindex++, i++) {

        if(c == '.') {
            has_fraction = 1;
            continue;
        }

        if(has_fraction)
            power *= 10;
        dig *= 10;
        dig += c - '0';
    }

    return dig / power;
}

static char* scan_identifier(char c) {
    unsigned i;
    char* identifier = malloc(MAXIDENTIFIER * sizeof(char));
    identifier[0] = c;

    for(i = 1; i < MAXIDENTIFIER - 1 && isalnum(c = ptr[ptrindex]); ptrindex++, i++)
        identifier[i] = c;
    identifier[i] = '\0';

    return identifier;
}

static Token* token_create(TokenType type) {
    Token* token;
    token = malloc(sizeof(struct Token));
    token->literal.lexeme = NULL;
    token->type = type;
    return token;
}

static Token* token_create_lex(TokenType type, char* lexeme) {
    Token* token = token_create(type);
    token->literal.lexeme = strdup(lexeme);
    return token;
}

static Token* token_create_dig(TokenType type, double digit) {
    Token* token = token_create(type);
    token->literal.number = digit;
    return token;
}

static Token* token_next(unsigned char* err) {
    char c;
    char* str;
    Token* tkn;
    switch(c = scan_next()) {
        case '(':
            return token_create(LEFT_PAREN);
        case ')':
            return token_create(RIGHT_PAREN);
        case '[':
            return token_create(LEFT_BRACE);
        case ']':
            return token_create(RIGHT_BRACE);
        case ',':
            return token_create(COMMA);
        case '-':
            return token_create(MINUS);
        case '+':
            return token_create(PLUS);
        case '*':
            return token_create(STAR);
        case '/':
            return token_create(FSLASH);
        case '=':
            return token_create(EQUAL);
        case ' ':
        case '\r':
        case '\t':
            // ignore whitespace
            break;
        case '\n':
            // finish
            ptrindex = ptrlen;
            break;
        default:
            if(isdigit(c)) {
                return token_create_dig(NUMBER, scan_dig(c));
            } else if(isalpha(c)) {
                str = scan_identifier(c);
                if(strcmp(VARSTR, str) == 0) {
                    return token_create(VAR);
                } else if(strcmp(PRINTSTR, str) == 0) {
                    return token_create(PRINT);
                } else if(strcmp(QUITSTR, str) == 0) {
                    return token_create(QUIT);
                }
                tkn = token_create_lex(IDENTIFIER, str);
                free(str);
                return tkn;
            } else {
                printf("Error: invalid character %c\n", c);
                *err = 1;
                return NULL;
            }
            break;
    }
    *err = 0;
    return NULL;
}

TokenList* token_scan(char* line, ssize_t nchar) {
    unsigned char err;
    unsigned count = 0, length = DEFAULTSIZE;
    Token **tarr, *tkn;
    TokenList* tlist = malloc(sizeof(struct TokenList));
    tlist->arr = malloc(DEFAULTSIZE * sizeof(struct Token *));

    scan_init(line, nchar);
    while(ptrindex < ptrlen) {
        if(length <= count) {
            length *= 2;
            tarr = realloc(tlist->arr, length * sizeof(struct Token *));
            tlist->arr = tarr;
        }

        if((tkn = token_next(&err)) == NULL) {
            if(err) {
                token_destroy(tlist);
                return NULL;
            }
            continue;
        } else if(tkn->type == QUIT) {
            if(count != 0) {
                free(tkn);
                token_destroy(tlist);
                printf("Error: quit is a keyword. Exiting.\n");
            }

            tlist->arr[0] = tkn;
            tlist->length = length;
            tlist->count = 1;
            return tlist;
        }
        tlist->arr[count++] = tkn;
    }

    tlist->length = length;
    tlist->count = count;
    return tlist;
}

void token_destroy(TokenList* tlist) {
    unsigned i;
    for(i = 0; i < tlist->count; i++) {
        if(tlist->arr[i]->type == IDENTIFIER)
            free(tlist->arr[i]->literal.lexeme);
        free(tlist->arr[i]);
    }
    free(tlist->arr);
    free(tlist);
}
