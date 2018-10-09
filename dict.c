/*
 *  Hash table design:
 *  http://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html?highlight=%28CategoryAlgorithmNotes%29
 *
 *  Slight modifications and adjustments made from original 
 *  to suite this applications needs.
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dict.h"

struct elt {
    struct elt* next;
    char* key;
    int* value;
};

struct dict {
    int size; /* size of pointer table*/
    int n; /* number of elements stored */
    struct elt** table;
};

#define INITIAL_SIZE (1024)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)

Dict internalDictCreate(int size) {
    Dict d;
    int i;

    d = malloc(sizeof(*d));
    assert(d != NULL);

    d->size = size;
    d->n = 0;
    d->table = malloc(sizeof(struct elt *) * size);

    assert(d->table != NULL);

    for(i = 0; i < size; i++)
        d->table[i] = 0;

    return d;
}

Dict DictCreate(void) {
    return internalDictCreate(INITIAL_SIZE);
}

void DictDestroy(Dict d) {
    int i;
    struct elt* e;
    struct elt* next;

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = next) {
            next = e->next;

            free(e->key);
            free(e->value);
            free(e);
        }
    }

    free(d->table);
    free(d);
}

#define MULTIPLIER (97)

static unsigned long hash_function(const char* s) {
    unsigned const char* us;
    unsigned long h;

    h = 0;

    for(us = (unsigned const char *) s; *us; us++) {
        h = h * MULTIPLIER + *us;
    }

    return h;
}

static void grow(Dict d) {
    Dict d2;
    struct dict swap;
    int i;
    struct elt* e;

    d2 = internalDictCreate(d->size * GROWTH_FACTOR);

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = e->next) {
            DictInsert(d2, e->key, e->value);
        }
    }

    swap = *d;
    *d = *d2;
    *d2 = swap;

    DictDestroy(d2);
}

void DictInsert(Dict d, const char* key, int* value) {
    struct elt* e;
    unsigned long h;

    assert(key);
    assert(value);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = strdup(key);
    e->value = value;

    h = hash_function(key) % d->size;

    e->next = d->table[h];
    d->table[h] = e;

    d->n++;

    if(d->n >= d->size * MAX_LOAD_FACTOR) {
        grow(d);
    }
}

int* DictSearch(Dict d, const char* key) {
    struct elt* e;

    for(e = d->table[hash_function(key) % d->size]; e != 0; e = e->next) {
        if(!strcmp(e->key, key)) {
            return e->value;
        }
    }
    
    return NULL;
}

void DictDelete(Dict d, const char* key) {
    struct elt** prev; /* what to change when elt is deleted */
    struct elt* e; /* what to delete */

    for(prev = &(d->table[hash_function(key) % d->size]);
            *prev != 0;
            prev = &((*prev)->next)) {
        if(!strcmp((*prev)->key, key)) {
            e = *prev;
            *prev = e->next;

            free(e->key);
            free(e->value);
            free(e);

            return;
        }
    }
}
