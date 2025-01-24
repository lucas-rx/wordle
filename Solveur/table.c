#include "table.h"

int hash(char *some_value)
{
    int res = 0;
    int i = 0;

    for (i = 0; some_value[i] != '\0'; i++)
    {
        res = res + func(some_value[i]);
    }
    return res;
}

int func(char chr) {
    return chr;
}

int power(int base, int exposant) {
    
    // Retourne power ^ exposant
    
    int res = 1;
    for (int i = 0; i < exposant; i++) {
        res *= base;
    }
    return res;
}

table_t *table_create(int size) {
    table_t * T = calloc(1,sizeof(table_t));
    T->size = size;
    T->L = calloc(T->size,sizeof(list_t*));
    //printf("%d\n",T->size);
    for (int i = 0;i<T->size;i++) {
        T->L[i] = list_create();
    }
    return T;
}

void table_destroy(table_t *one_table) {
    for (int i = 0;i<one_table->size;i++) {
        list_destroy(one_table->L[i]);
    }
    free(one_table->L);
    free(one_table);


}

int table_indexof(table_t *one_table, char *one_key) {
    int h = hash(one_key);
    int size = one_table->size;
    int ind = h % size;
    return ind;
}

bool table_add(table_t *one_table, char *one_key) {
    int ind = table_indexof(one_table,one_key);
    //printf("%d\n",ind);
    if (list_contains(one_table->L[ind],one_key)) {
        return false;
    }
    else {
        list_append(one_table->L[ind],one_key);
        return true;
    }

}

bool table_contains(table_t *one_table, char *one_key) {
    int ind = table_indexof(one_table,one_key);
    if (list_contains(one_table->L[ind],one_key)) {
        return true;
    }
    return false;



}

char *table_get(table_t *one_table, char *one_key) {
    int ind = table_indexof(one_table,one_key);
    char * res = list_find(one_table->L[ind],one_key);
    return res;

}

void table_print(table_t *t){
    for (int i=0; i<t->size;i++){
        list_print(t->L[i]);
    }
}

void table_remove(table_t *t, char *mot){
    for (int i=0; i<t->size;i++){
        if (list_contains(t->L[i],mot)){
            list_remove(t->L[i],mot);
        }
    }
}

int table_empty(table_t *t){
    for (int i=0; i<t->size;i++){
        if (!list_is_empty(t->L[i])){
            return 0;
        }
    }
    return 1;
}


