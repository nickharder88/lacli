#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "../defs.h"
#include "scanner.h"
#include "error.h"
#include "../dict.h"

#define VARSTR "var"
#define DEFAULTSIZE 8

static char* ptr;
static unsigned ptrlen;
static unsigned ptrindex;

static TokenType is_keyword(char* identifier) {
    return strcmp(identifier, "var") == 0;
}

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

static char scan_peek() {
    if(ptrindex < ptrlen) {
        return ptr[ptrindex];
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

static Token* token_next(void) {
    char c;
    char* str;
    double dig;
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
                if(strcmp(VARSTR, str))
                    return token_create(VAR);
                return token_create_lex(IDENTIFIER, str);
            } else {
                errlog("Unexpected character");
                // TODO error handle
                return NULL;
            }
            break;
    }
    return NULL;
}

TokenList* token_scan(char* line, unsigned len) {
    unsigned i, size, length = DEFAULTSIZE;
    Token* tarr;
    TokenList* tlist = malloc(sizeof(struct TokenList));
    tlist->arr = malloc(DEFAULTSIZE * sizeof(struct Token));

    scan_init(line, len);
    for(i = 0; ptrindex < ptrlen; i++) {
        if(length <= i) {
            length *= 2;
            tarr = realloc(tlist->arr, length * sizeof(struct Token));
            tlist->arr = tarr;
        }

        if((tarr = token_next()) == NULL) {
            break;
        }
        tlist->arr[i] = *tarr;
    }

    tlist->length = length;
    tlist->count = i;
    return tlist;
}

void token_destroy(TokenList* tlist) {
    unsigned i;
    Token* ptr;
    for(i = 0; i < tlist->count; i++) {
        ptr = tlist->arr + i;
        if(ptr->type == IDENTIFIER)
            free(ptr->literal.lexeme);
        free(ptr);
    }
    free(tlist);
}
