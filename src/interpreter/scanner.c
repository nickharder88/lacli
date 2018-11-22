#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "../defs.h"
#include "scanner.h"
#include "error.h"
#include "../dict.h"

#define VARSTR "var"

static char* ptr;
static unsigned ptrlen;
static unsigned ptrindex;

static TokenType is_keyword(char* identifier) {
    return strcmp(identifier, "var") == 0;
}

static void scan_init(char* line, unsigned len) {
    ptr = line;
    len = ptrlen;
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

static double scan_dig(void) {
    char c, has_fraction;
    unsigned i, power = 1;
    double dig;

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

static char* scan_identifier(void) {
    char c;
    unsigned i;
    char* identifier = malloc(MAXIDENTIFIER * sizeof(char));

    for(i = 0; i < MAXIDENTIFIER - 1 && isalnum(c = ptr[ptrindex]); ptrindex++, i++)
        identifier[i] = c;

    return identifier;
}

static TokenNode* token_create(TokenType type) {
    TokenNode* token;
    token = malloc(sizeof(struct Token));
    token->literal.lexeme = NULL;
    token->type = type;
    token->next = NULL;
    return token;
}

static TokenNode* token_create_lex(TokenType type, char* lexeme) {
    TokenNode* token = token_create(type);
    token->literal.lexeme = strdup(lexeme);
    return token;
}

static TokenNode* token_create_dig(TokenType type, double digit) {
    TokenNode* token = token_create(type);
    token->literal.number = digit;
    return token;
}

static TokenNode* token_next(void) {
    char c;
    char* str;
    double dig;
    TokenNode* token;

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
        default:
            if(isdigit(c)) {
                token_create_dig(NUMBER, scan_dig());
            } else if(isalpha(c)) {
                str = scan_identifier();
                if(strcmp(VARSTR, str))
                    return token_create(VAR);
                return token_create_lex(IDENTIFIER, str);
            } else {
                errlog("Unexpected character");
                return NULL;
            }
            break;
    }

    return token;
}

TokenNode* token_scan(char* line, unsigned len) {
    TokenNode *head, *ptr;
    head = ptr = token_create(NONE);

    scan_init(line, len);

    while(*line != '\n' && *line != '\0' && *line != EOF) {
        ptr->next = token_next();
        ptr = ptr->next;
    }

    ptr = head->next;
    free(head);
    return ptr;
}

void token_destroy(TokenNode* token) {
    TokenNode* ptr;
    while(token) {
        if(token->type == IDENTIFIER)
            free(token->literal.lexeme);
        ptr = token;
        token = token->next;
        free(ptr);
    }
}
