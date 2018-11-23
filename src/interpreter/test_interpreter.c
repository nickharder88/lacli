#include "parser.h"

int main() {
    ssize_t numchar;
    char *line = NULL;
    size_t len = 0;
    TokenList* tlist;
    Expr* expr;

    while(1) {
       if((numchar = getline(&line, &len, stdin)) == -1) {
           continue;
       }

       tlist = token_scan(line, numchar);
       expr = parse(tlist);
    }
}
