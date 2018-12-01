#include "repl.h"
#include "scanner.h"
#include "parser.h"
#include "evaluate.h"
#include "environment.h"
#include "funcs.h"

void repl_init(void) {
    env_init();
    funcs_init();
}

void repl_destroy(void) {
    env_destroy();
    funcs_destroy();
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

    token_destroy(tlist);
    stmt_destroy(statement);
    return 1;
}
