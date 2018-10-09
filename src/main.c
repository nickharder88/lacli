#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"

int main(int argc, char** argv) {
    Matrix matrix;
    char line[200];

    do {
        printf("Enter matrix:\n");
        fgets(line, 200, stdin);
        matrix = matrix_parse(line);
        if(matrix == NULL) {
            break;
        }

        matrix_print(matrix, stdout);
        matrix_destroy(matrix);
    } while(1);

    return 0;
}
