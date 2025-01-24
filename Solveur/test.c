#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dico.h"

int main() {

    /* TEST STRUCTURES DE DONNES */
    table_t *T = table_create(4);
    assert(table_add(T,"Vous") == true);
    assert(table_add(T,"fonction") == true);
    assert(table_add(T,"table") == true);
    assert(table_add(T,"hachage") == true);
    assert(table_add(T,"prendre") == true);
    assert(table_add(T,"conjuguer") == true);
    assert(table_add(T,"tirer") == true);
    assert(table_add(T,"ineluctable") == true);
    assert(table_add(T,"je") == true);
    assert(table_add(T,"nous") == true);
    assert(table_add(T,"Vous") == false);
    assert(table_add(T,"ineluctable") == false);


    assert(table_contains(T,"fonction") == true);
    assert(table_contains(T,"table") == true);
    assert(table_contains(T,"hachage") == true);
    assert(table_contains(T,"prendre") == true);
    assert(table_contains(T,"conjuguer") == true);
    assert(table_contains(T,"tirer") == true);
    assert(table_contains(T,"ineluctable") == true);
    assert(table_contains(T,"je") == true);
    assert(table_contains(T,"nous") == true);
    assert(table_contains(T,"Vous") == true);

    assert(strcmp(table_get(T,"fonction"),"fonction") == 0);
    assert(strcmp(table_get(T,"Vous"),"Vous") == 0);
    assert(strcmp(table_get(T,"ineluctable"),"ineluctable") == 0);
    table_destroy(T);
    
    /*
    /* TEST SOLVEUR */
    /*
    table_t *Th = table_create(300);
    assert(table_add(Th,"epier") == true);
    assert(table_add(Th,"berce") == true);
    assert(table_add(Th,"caser") == true);
    assert(table_add(Th,"enfui") == true);
    assert(table_add(Th,"bicot") == true);
    assert(table_add(Th,"bouse") == true);
    assert(table_add(Th,"verni") == true);
    assert(table_add(Th,"vibre") == true);
    assert(table_add(Th,"essor") == true);
    assert(table_add(Th,"boche") == true);
    char *m = "epier";
    char *p = "00000";
    list_t *L = list_create();
    suitable(m,p,Th,L,0);
    assert(list_is_empty(L) == true);
    list_destroy(L);

    char *e = "bicot";
    char *pe = "21000";
    list_t *L1 = list_create();
    suitable(e,p,Th,L1,0);
    list_print(L1);
    list_destroy(L1);
    table_destroy(Th);
    */

}
