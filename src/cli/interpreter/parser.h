#ifndef PARSER_H
#define PARSER_H

#include "expr.h"
#include "stmt.h"
#include "scanner.h"

Stmt* parse(TokenList* tokenlist);

#endif
