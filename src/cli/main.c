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
    char *ifile_name;
    char *line = NULL, quit = 0, has_ifile = 0;
    size_t len = 0;
    FILE *ifile;

    repl_code rc = REPL_SUCCESS;

    printf("LACLI 1.0\n\n");

    if(argc == 1) {
        ifile = stdin;
    } else if(argc == 2) {
        has_ifile = 1;
        if(argc == 2) {
            ifile_name = argv[1];
            if((ifile = fopen(ifile_name, "r")) == NULL) {
                printf("Error: could not open file: %s\n", ifile_name);
                return 1;
            }
        }
    } else {
        printf("Error: no arguements for stdin, 1 argument for file input\n");
        return 1;
    }

    repl_init();

    if(has_ifile) {
        while(rc != REPL_QUIT && !feof(ifile)) {
            if((nchar = getline(&line, &len, ifile)) == -1) {
                /* skipped line */
                continue;
            }

            rc = repl(line, nchar);
        }

        /* close opened file */
        fclose(ifile);
    } else {
        while(rc != REPL_QUIT && !feof(ifile)) {
            promptf("", 0);
            if((nchar = getline(&line, &len, ifile)) == -1) {
                /* skipped line */
                continue;
            }

            rc = repl(line, nchar);
        }
    }
    
    repl_destroy();

    free(line);
    return 0;
}
