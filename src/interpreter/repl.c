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

    if((statement = parse(tlist)) == NULL) {
        // skip
        return 1;
    }

    evaluate(statement);
    return 1;
}
