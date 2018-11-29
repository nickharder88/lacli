#include "repl.h"
#include "scanner.h"
#include "parser.h"
#include "evaluate.h"

void* repl(char* line) {
    unsigned len;

    TokenList* tlist;
    Stmt* statement;

    tlist = token_scan(line, len);
    statement = parse(tlist);
    evaluate(statement);
}
