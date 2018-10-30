#ifndef ROW_H
#define ROW_H

typedef struct Row {
    unsigned len;
    int *vals;
} Row;

void row_destroy(Row *r);
Row* row_parse(char** line);

#endif
