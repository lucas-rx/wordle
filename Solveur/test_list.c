#include <assert.h>

#include "list.h"

int main(){
    printf("---Début des tests---\n");
    list_t *l=list_create();
    assert(list_size(l)==0);
    assert(list_is_empty(l)==1);

    list_append(l, "key1");
    list_append(l, "key2");
    list_append(l, "key3");
    list_append(l, "key4");
    assert(list_size(l)==4);

    list_remove(l,"key1");
    list_remove_first(l);
    assert(list_size(l)==2);


    list_destroy(l);
    assert(list_is_empty(l)==0);

    list_t *g=list_create();

    list_append(g, "key1");
    list_append(g, "key2");
    list_append(g, "key3");
    list_append(g, "key4");
    
    list_destroy_index(g,2);

    assert(list_size(g)==2);  
    assert(list_is_empty(g)==0);

    assert(list_contains(g, "key1")==0);
    assert(list_contains(g, "key4")==1);

    assert(list_find(g,"key11")==NULL);

    assert(strcmp(list_first(g)->ch1,"key3")==0);

    list_remove(l,"key4");
    assert(list_contains(g, "key4")==0);

    printf("---Fin des tests---\n");

    return 0;
}