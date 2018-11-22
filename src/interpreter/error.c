#include <stdio.h>

#include "error.h"

void errlog(char* message) {
    fprintf(stderr, "%s\n", message);
}
