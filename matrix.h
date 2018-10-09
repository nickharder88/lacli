#define MAXLEN_NAME 21

typedef struct Matrix* Matrix;

Matrix matrix_create(char* name, int rows, int cols);

void matrix_change(Matrix m, int row, int col, int val);

void matrix_destroy(Matrix m);
