#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#ifndef __LIST_H__
#define __LIST_H__

struct _element_t
{
    char *ch1;
    struct _element_t *suiv;

};

typedef struct _element_t element_t;

struct _list_t
{
    element_t *head;

};

typedef struct _list_t list_t;

list_t *list_create();
int list_size(list_t* one_list);
element_t * list_get(list_t* one_list, int index);
element_t * list_first(list_t* one_list);
element_t * list_last(list_t * one_list);

void list_remove(list_t* one_list, char* one_str);
void list_remove_first(list_t* one_list);

void list_destroy(list_t *one_list);
void list_destroy_index(list_t *one_list,int k);

bool list_is_empty(list_t *one_list);

void list_append(list_t *one_list, char *one_key);

void element_print(element_t *one_element);

void list_print(list_t *one_list);

bool list_contains(list_t *one_list, char *one_key);

char *list_find(list_t *one_list, char *one_key);

#endif /* __LIST_H__ */


