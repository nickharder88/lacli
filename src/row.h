#ifndef ROW_H
#define ROW_H

typedef struct Row {
    unsigned len;
    double *vals;
} Row;

void row_destroy(Row *r);
Row* row_parse(char** line);

void row_multiply(Row* a, Row* b, double scalar);
void row_scale(Row* a, double scalar);

Row* row_copy(Row* r);

#endif
