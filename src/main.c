#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"
#include "interpreter/repl.h"

static void promptf(char* print, int n) {
    if(n == 0) {
        printf("> ");
        return;
    }

    printf("%s\n> ", print);
}

int main(int argc, char** argv) {
    /* Allocated buffer size */
    ssize_t nchar;
    char *line = NULL, quit = 0;
    size_t len = 0;

    printf("LACLI 1.0\n\n");

    repl_init();

    while(!quit) {
        promptf("", 0);
        if((nchar = getline(&line, &len, stdin)) == -1) {
            /* skipped line */
            continue;
        }

        if(!repl(line, nchar)) {
            break;
        }
    }

    repl_destroy();

    free(line);
    return 0;
}
