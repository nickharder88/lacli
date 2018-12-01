#ifndef REPL_H
#define REPL_H

#include <sys/types.h>

void repl_init(void);
void repl_destroy(void);
char repl(char* line, ssize_t nchar);

#endif
