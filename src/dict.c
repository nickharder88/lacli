#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "matrix.h"

typedef struct nlist {
    struct nlist* next;
    struct nlist* prev;
    char* key;
    Matrix* matrix;
} nlist;

#define HASHSIZE 101
static nlist* dict[HASHSIZE];

Matrix* _dict_next(char restart) {
    static unsigned i = 0;
    static nlist *np = NULL;
    Matrix* m;

    if(restart) {
        i = 0;
        np = NULL;
        return NULL;
    }

    while(np == NULL && i < HASHSIZE) {
        i++;
        np = dict[i];
    }

    /* need to restart iterator */
    if(i == HASHSIZE) {
        return NULL;
    }

    m = np->matrix;
    np = np->next;
    return m;
}

void dict_iter_begin(void) {
    _dict_next(1);
}

Matrix* dict_next(void) {
    return _dict_next(0);
}

unsigned hash(char* s) {
    unsigned hashval;
    for(hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

nlist* dict_lookup(char* key) {
    for(nlist* np = dict[hash(key)]; np != NULL; np = np->next)
        if(strcmp(key, np->key) == 0)
            return np; /* found */
    return NULL; /* not found */
}

Matrix* dict_get(char* key) {
    nlist* np = dict_lookup(key);
    if(np == NULL)
        return NULL;
    return np->matrix;
}

void *dict_add(char* key, Matrix* val) {
    nlist *np, *next;
    unsigned hashval;

    if((np = dict_lookup(key)) == NULL) {
        np = malloc(sizeof(nlist));
        if(np == NULL || (np->key = strdup(key)) == NULL) {
            free(np);
            return NULL;
        }
        np->matrix = val;

        hashval = hash(key);
        next = dict[hashval];
        np->next = next;
        np->prev = NULL;
        if(next != NULL)
            next->prev = np;
        dict[hashval] = np;
    } else {
        /* Already have this key
         * Destroy old matrix and put in new one 
         */
        matrix_destroy(np->matrix);
        np->matrix = val;
    }

    /* if strdup fails */
    if((np->key = strdup(key)) == NULL) {
        free(np);
        return NULL;
    }

    return np;
}

Matrix* dict_remove(char* key) {
    nlist *np, *prev, *next;
    unsigned hashval;
    Matrix* m;

    /* not in dict */
    if((np = dict_lookup(key)) == NULL) {
        return NULL;
    }

    hashval = hash(key);
    prev = np->prev;
    next = np->next;

    /* head of list */
    if(prev == NULL) {
        if(next != NULL) {
            next->prev = NULL;
        }
        dict[hashval] = next;
    }

    m = next->matrix;
    free(np->key);
    free(np);

    return m;
}
