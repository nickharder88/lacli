#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "matrix.h"

struct pair {
    char *key;
    Matrix matrix;
};
typedef struct pair* pair;

struct nlist {
    struct nlist* next;
    struct nlist* prev;
    char* key;
    Matrix matrix;
};

unsigned hash(char* s) {
    unsigned hashval;
    for(hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval;
}

nlist dict_lookup(char* key) {
    nlist np;

    for(np = dict[hash(key)]; np != NULL; np = np->next)
        if(strcmp(key, np->key) == 0)
            return np; /* found */
    return NULL; /* not found */
}

Matrix dict_get(char* key) {
    nlist np;
    np = dict_lookup(key);
    if(np == NULL)
        return NULL;
    return np->matrix;
}

void *dict_add(char* key, Matrix val) {
    nlist np;
    unsigned hashval;

    if((np = dict_lookup(key)) == NULL) {
        np = malloc(sizeof(nlist));
        if(np == NULL || (np->key = strdup(key)) == NULL) {
            free(np);
            return NULL;
        }
        hashval = hash(key);
        np->next = dict[hashval];
        np->prev = NULL;
        dict[hashval]->prev = np;
        dict[hashval] = np;
    } else
        matrix_destroy(np->matrix);

    /* if strdup fails */
    if((np->key = strdup(key)) == NULL) {
        free(np);
        return NULL;
    }

    return np;
}

Matrix dict_remove(char* key) {
    nlist np, prev, next;
    unsigned hashval;
    Matrix m;

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
