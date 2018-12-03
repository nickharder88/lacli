#ifndef SPAN_H
#define SPAN_H

/*
 * Let w1,...,wk and v be vectos in Rn.
 * Then v is in span{w1,...,wk} iff the system of
 * linear equations Ar=v has a solution where A
 * is (w1|w2|...|wk)
 */

Rval* span_handler(Rval** args, unsigned nargs);
Rval* span(Matrix* v, Matrix** columns);

#endif
