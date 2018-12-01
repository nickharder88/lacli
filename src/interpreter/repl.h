#ifndef REPL_H
#define REPL_H

#include <sys/types.h>

typedef enum {
    REPL_ERROR, REPL_SUCCESS, REPL_QUIT
} repl_code;

void repl_init(void);
void repl_destroy(void);
repl_code repl(char* line, ssize_t nchar);

#endif
