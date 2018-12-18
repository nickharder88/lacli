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
 * Returns 
 * 0 if success
 * 1 if quit
 * 2 if recoverable error
 * 3 if unrecoverable error
 */
repl_code repl(char* line, ssize_t nchar) {
    TokenList* tlist;
    Stmt* statement;

    if((tlist = token_scan(line, nchar)) == NULL) {
        return REPL_ERROR;
    } else if(tlist->count == 0) {
        token_destroy(tlist);
        return REPL_SUCCESS;
    } else if(tlist->arr[0]->type == QUIT) {
        token_destroy(tlist);
        return REPL_QUIT;
    }

    if((statement = parse(tlist)) == NULL) {
        token_destroy(tlist);
        return REPL_ERROR;
    }

    evaluate(statement);

    token_destroy(tlist);
    stmt_destroy(statement);
    return REPL_SUCCESS;
}
