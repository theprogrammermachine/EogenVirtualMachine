#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "Dictionary.h"

#define INITIAL_SIZE (1024)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)

/* Dictionaryionary initialization code used in both DictionaryCreate and grow */
Dictionary*
internalDictionaryCreate(int size)
{
    Dictionary* d;
    int i;

    d = malloc(sizeof(*d));

    assert(d != 0);

    d->size = size;
    d->n = 0;
    d->table = malloc(sizeof(struct elt *) * d->size);

    assert(d->table != NULL);

    for(i = 0; i < d->size; i++) d->table[i] = NULL;

    return d;
}

Dictionary*
dict_new()
{
    return internalDictionaryCreate(INITIAL_SIZE);
}

void
dict_free(Dictionary* d)
{
    int i;
    struct elt *e;
    struct elt *next;

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

static unsigned long
hash_function(const char *s)
{
    unsigned const char *us;
    unsigned long h;

    h = 0;

    for(us = (unsigned const char *) s; *us; us++) {
        h = h * MULTIPLIER + *us;
    }

    return h;
}

static void
grow(Dictionary* d)
{
    Dictionary* d2;            /* new Dictionaryionary we'll create */
    Dictionary* swap;   /* temporary structure for brain transplant */
    int i;
    struct elt *e;

    d2 = internalDictionaryCreate(d->size * GROWTH_FACTOR);

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != NULL; e = e->next) {
            /* note: this recopies everything */
            /* a more efficient implementation would
             * patch out the strdups inside DictionaryInsert
             * to avoid this problem */
            dict_add(d2, e->key, e->value);
        }
    }

    /* the hideous part */
    /* We'll swap the guts of d and d2 */
    /* then call DictionaryDestroy on d2 */
    swap = d;
    *d = *d2;
    d2 = swap;

    dict_free(d2);
}

/* insert a new key-value pair into an existing Dictionaryionary */
void
dict_add(Dictionary* d, char *key, void *value)
{
    struct elt *e;
    unsigned long h;

    assert(key);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = strdup(key);
    e->value = value;

    h = hash_function(key) % d->size;

    e->next = d->table[h];
    d->table[h] = e;

    d->n++;

    /* grow table if there is not enough room */
    if(d->n >= d->size * MAX_LOAD_FACTOR) {
        grow(d);
    }
}

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
void *
dict_get(Dictionary* d, char *key)
{
    struct elt *e;

    for(e = d->table[hash_function(key) % d->size]; e != NULL; e = e->next) {
        if(!strcmp(e->key, key)) {
            return e->value;
        }
    }

    return NULL;
}

void
display_keys(Dictionary* d) {
    printf("\n==================================================\n");
    for(int i = 0; i < d->size; i++) {
        struct elt *e = d->table[i];
        while (e != NULL) {
            printf("\nkey : %s\n", e->key);
            e = e->next;
        }
    }
    printf("\n==================================================\n");
}

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void
dict_delete(Dictionary* d, const char *key)
{
    struct elt **prev;          /* what to change when elt is deleted */
    struct elt *e;              /* what to delete */

    for(prev = &(d->table[hash_function(key) % d->size]);
        *prev != 0;
        prev = &((*prev)->next)) {
        if(!strcmp((*prev)->key, key)) {
            /* got it */
            e = *prev;
            *prev = e->next;

            free(e->key);
            free(e->value);
            free(e);

            return;
        }
    }
}