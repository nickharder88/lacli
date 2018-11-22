#ifndef SCANNER_H
#define SCANNER_H

typedef enum {
    // Single character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, PLUS, MINUS, STAR, FSLASH, EQUAL,

    // Literals
    IDENTIFIER, NUMBER,

    // KEYWORDS
    VAR, NONE
} TokenType;

typedef struct Token {
    TokenType type;
    union {
        char* lexeme;
        double number;
    } literal;
    struct Token* next;
} TokenNode;

TokenNode* token_scan(char* line, unsigned len);
void token_destroy(TokenNode* token);

#endif
