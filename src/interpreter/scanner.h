#ifndef SCANNER_H
#define SCANNER_H

typedef enum {
    // Single character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, PLUS, MINUS, STAR, FSLASH, EQUAL,

    // Literals
    IDENTIFIER, NUMBER,

    // KEYWORDS
    VAR, PRINT
} TokenType;

typedef struct Token {
    TokenType type;
    union {
        char* lexeme;
        double number;
    } literal;
} Token;

typedef struct TokenList {
    unsigned count;
    unsigned length;
    Token* arr;
} TokenList;

TokenList* token_scan(char* line, unsigned len);
void token_destroy(TokenList* tlist);

#endif
