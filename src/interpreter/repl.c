#include "repl.h"
#include "scanner.h"
#include "parser.h"
#include "evaluate.h"

void repl(void) {
    unsigned len;
    char* line;

    TokenList* tlist;
    Stmt* statement;

    tlist = token_scan(line, len);
    statement = parse(tlist);
    evaluate(statement);
}
