#include "repl.h"
#include "scanner.h"
#include "parser.h"
#include "evaluate.h"
#include "environment.h"

void repl_init(void) {
    env_init();
}

void repl(char* line, ssize_t nchar) {
    TokenList* tlist;
    Stmt* statement;

    tlist = token_scan(line, nchar);
    statement = parse(tlist);
    evaluate(statement);
}
