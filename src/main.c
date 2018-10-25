#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "matrix.h"
#include "dict.h"

void promptf(char* print, int n) {
    if(n == 0) {
        printf("==> ");
        return;
    }

    printf("%s\n==> ", print);
}

int get_command(char* command, char* line) {
    int i;
    for(i = 0; i < MAXIDENTIFIER - 1; i++) {
        if(isalnum(*line)) {
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
    char *line = NULL, *ptr;
    char command[MAXIDENTIFIER];
    size_t len = 0;

    Matrix matrix;

    while(1) {
        promptf("", 0);
        if((numchar = getline(&line, &len, stdin)) == -1) {
            continue;
        }

        numchar = get_command(command, line);
        /* move pointer */
        line += numchar;

        /* skip whitespace after identifier */
        for(ptr = line; *ptr == ' ' || *ptr == '\t'; ptr++);

        switch(*line) {
            case '\n':
            case '\0':
                matrix = dict_get(command);
                matrix_print(matrix, stdout);
                break;
            case '=':
                matrix = matrix_parse(command, line);
                dict_add(command, matrix);
                break;
            default:
                printf("Invalid input\n");
                free(line);
                return 1;
        }
    }

    /* free each matrix in dict */

    free(line);
    return 0;
}
