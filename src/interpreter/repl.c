#include "repl.h"
#include "scanner.h"
#include "parser.h"
#include "evaluate.h"
#include "environment.h"

void repl_init(void) {
    env_init();
}

/*
 * Returns 0 if user wants to exit
 */
char repl(char* line, ssize_t nchar) {
    TokenList* tlist;
    Stmt* statement;

    if((tlist = token_scan(line, nchar)) == NULL) {
        return 0;
    }

    statement = parse(tlist);
    evaluate(statement);
    return 1;
}
