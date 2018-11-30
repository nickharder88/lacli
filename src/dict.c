#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "matrix.h"

#define HASHSIZE 101

typedef struct nlist {
    struct nlist* next;
    struct nlist* prev;
    char* key;
    void* data;
} nlist;

struct Dict {
    void (*destroy)(void *);
    nlist* table[HASHSIZE];
};

Dict* dict_create(void (*destroy)(void*)) {
    Dict* d = malloc(sizeof(struct Dict));
    d->destroy = destroy;
    return d;
}

void* _dict_next(Dict* d, char restart) {
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
        np = d->table[i];
    }

    /* need to restart iterator */
    if(i == HASHSIZE) {
        return NULL;
    }

    m = np->data;
    np = np->next;
    return m;
}

void dict_iter_begin(Dict* d) {
    _dict_next(d, 1);
}

void* dict_next(Dict* d) {
    return _dict_next(d, 0);
}

unsigned hash(char* s) {
    unsigned hashval;
    for(hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

nlist* dict_lookup(Dict* d, char* key) {
    for(nlist* np = d->table[hash(key)]; np != NULL; np = np->next)
        if(strcmp(key, np->key) == 0)
            return np; /* found */
    return NULL; /* not found */
}

void* dict_get(Dict* d, char* key) {
    nlist* np = dict_lookup(d, key);
    if(np == NULL)
        return NULL;
    return np->data;
}

void* dict_add(Dict* d, char* key, void* val) {
    nlist *np, *next;
    unsigned hashval;

    if((np = dict_lookup(d, key)) == NULL) {
        np = malloc(sizeof(nlist));
        if(np == NULL || (np->key = strdup(key)) == NULL) {
            free(np);
            return NULL;
        }
        np->data = val;

        hashval = hash(key);
        next = d->table[hashval];
        np->next = next;
        np->prev = NULL;
        if(next != NULL)
            next->prev = np;
        d->table[hashval] = np;
    } else {
        /* Already have this key */
        d->destroy(np->data);
        np->data = val;
    }

    /* if strdup fails */
    if((np->key = strdup(key)) == NULL) {
        free(np);
        return NULL;
    }

    return np->data;
}

void dict_add_range(Dict* d, char* keys, void* vals, unsigned len) {
    unsigned i;
    for(i = 0; i < len; i++)
        dict_add(d, keys + i, vals + i);
}

void* dict_remove(Dict* d, char* key) {
    nlist *np, *prev, *next;
    unsigned hashval;
    void* data;

    /* not in Dict* */
    if((np = dict_lookup(d, key)) == NULL) {
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
        d->table[hashval] = next;
    }

    data = np->data;
    free(np->key);
    free(np);

    return data;
}

void dict_clear(Dict* d) {
    nlist *np, *next;
    unsigned i;

    if(d->destroy == NULL) {
        return;
    }

    for(i = 0; i < HASHSIZE; i++) {
        np = d->table[i];

        while(np != NULL) {
            next = np->next;
            d->destroy(np->data);
            free(np->key);
            free(np);
            np = next;
        }
    }
}
