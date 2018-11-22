#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "matrix.h"
#include "dict.h"
#include "funcs/funcs.h"

#define QUIT "quit"

static void promptf(char* print, int n) {
    if(n == 0) {
        printf("==> ");
        return;
    }

    printf("%s\n==> ", print);
}

static int get_command(char* command, char* line) {
    int i;

    for(i = 0; i < MAXIDENTIFIER - 1 && isalnum(line[i]); i++)
        command[i] = line[i];
    command[i] = '\0';
    return i;
}

int main(int argc, char** argv) {
    /* Allocated buffer size */
    ssize_t numchar;
    char *line = NULL, *ptr, quit = 0;
    char command[MAXIDENTIFIER];
    size_t len = 0;
    void* func;

    Matrix *matrix, *tmp;
    Dict *matrix_dict, *func_dict;

    matrix_dict = dict_create(matrix_destroy);
    func_dict = func_create();

    while(!quit) {
        promptf("", 0);
        if((numchar = getline(&line, &len, stdin)) == -1) {
            continue;
        }

        /* skip initial whitespace */
        for(; *line == ' ' || *line == '\t'; line++);

        numchar = get_command(command, line);

        /* skip whitespace after identifier */
        for(ptr = line + numchar; *ptr == ' ' || *ptr == '\t'; ptr++);

        switch(*ptr) {
            case '(':
                /* Call a function */
                if((func = is_func(func_dict, command))) {
                    call_func(func, matrix_dict, ptr);
                    break;
                }
                printf("Error: Invalid function name.\n");
                break;
            case '\n':
            case '\0':
                /* Exit the program */
                if(strcmp(command, QUIT) == 0) {
                    quit = 1;
                    break;
                }

                /* Print matrix */
                matrix = dict_get(matrix_dict, command);
                if(matrix == NULL) {
                    printf("Error: No matrix %s\n", command);
                    break;
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
                    matrix = matrix_evaluate(matrix_dict, command, ptr);

                if(matrix == NULL) {
                    break;
                }

                if(dict_get(matrix_dict, command) != NULL) {
                    tmp = dict_remove(matrix_dict, command);
                    matrix_destroy(tmp);
                }
                dict_add(matrix_dict, command, matrix);
                break;
            default:
                printf("Error: Invalid input\n");
                continue;
        }
    }

    dict_clear(matrix_dict);
    dict_clear(func_dict);
    free(line);
    return 0;
}
