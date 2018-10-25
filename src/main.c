#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "matrix.h"
#include "dict.h"

#define QUIT "quit"

void promptf(char* print, int n) {
    if(n == 0) {
        printf("==> ");
        return;
    }

    printf("%s\n==> ", print);
}

char* getword() {
}

int get_command(char* command, char* line) {
    int i;
    for(i = 0; i < MAXIDENTIFIER - 1; i++) {
        if(isalnum(line[i])) {
            command[i] = line[i];
            continue;
        }

        break;
    }
    command[i] = '\0';
    return i;
}

int main(int argc, char** argv) {
    /* Allocated buffer size */
    ssize_t numchar;
    char *line = NULL, *ptr, quit = 0;
    char command[MAXIDENTIFIER];
    size_t len = 0;

    Matrix matrix;

    while(!quit) {
        promptf("", 0);
        if((numchar = getline(&line, &len, stdin)) == -1) {
            continue;
        }

        numchar = get_command(command, line);

        /* skip whitespace after identifier */
        for(ptr = line + numchar; *ptr == ' ' || *ptr == '\t'; ptr++);

        switch(*ptr) {
            case '\n':
            case '\0':
                matrix = dict_get(command);
                if(matrix == NULL) {
                    printf("No matrix %s\n", command);
                    continue;
                }
                matrix_print(matrix);
                break;
            case '=':
                /* skip the = */
                ptr++;

                while(*ptr == ' ')
                    ptr++;

                if(*ptr == '[')
                    matrix = matrix_parse(command, ptr);
                else
                    matrix = matrix_evaluate(command, ptr);
                dict_add(command, matrix);

                break;
            default:
                /* Exit the program */
                if(strcmp(ptr, QUIT) == 0) {
                    quit = 1;
                    break;
                }
                printf("Invalid input\n");
                free(line);
                return 1;
        }
    }

    /* free each matrix in dict */
    dict_iter_begin();
    while((matrix = dict_next()) != NULL)
        matrix_destroy(matrix);

    free(line);
    return 0;
}
