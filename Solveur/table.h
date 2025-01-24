#include <math.h>

#include "list.h"

#define TAILLE_DICO 300

#ifndef __TABLE_H__
#define __TABLE_H__

struct _table_t {
    int size;
    list_t **L;
};

typedef struct _table_t table_t;


int hash(char *some_value);

int func(char chr);

int power(int base, int exposant);

table_t *table_create(int size);

void table_destroy(table_t *one_table);

int table_indexof(table_t *one_table, char *one_key);

bool table_add(table_t *one_table, char *one_key);

bool table_contains(table_t *one_table, char *one_key);

char *table_get(table_t *one_table, char *one_key);

void table_print(table_t *t);

void table_remove(table_t *t, char *mot);

int table_empty(table_t *t);

#endif /* __TABLE_H__ */
