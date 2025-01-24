#include "list.h"

list_t *list_create() {
    list_t *L = calloc(1,sizeof(list_t));
    return L;
}
void list_destroy_index(list_t *one_list,int k) {
    if (k == 0) {
        

    }
    if (!list_is_empty(one_list)) {
        int i = 0;
        while ((i<k-1)&&(one_list->head->suiv != NULL)) {
            element_t *elt_copy = one_list->head;
            one_list->head = one_list->head->suiv;
            free(elt_copy->ch1);
            free(elt_copy);
            i++;
        }
        element_t *elt_copy = one_list->head;
        one_list->head = one_list->head->suiv;
        free(elt_copy->ch1);
        free(elt_copy);
        
    }
    
    

}


void list_destroy(list_t *one_list) {
    if (!list_is_empty(one_list)) {
        while (one_list->head->suiv != NULL) {
            element_t *elt_copy = one_list->head;
            one_list->head = one_list->head->suiv;
            free(elt_copy->ch1);
            free(elt_copy);
        }
        free(one_list->head->ch1);
        free(one_list->head);
        free(one_list);
    }
    else {
        free(one_list->head);
        free(one_list);

    }
    

}

bool list_is_empty(list_t *one_list) {
    if (one_list->head == NULL) {
        return true;
    }
    return false;

}

int list_size(list_t* one_list) {
    if (list_is_empty(one_list)) {
        return 0;
    }
    element_t *copy = one_list->head;
    int i = 1;
    while(copy->suiv != NULL) {
        i++;
        copy = copy->suiv;
    }
    return i;

}

void list_append(list_t *one_list, char *one_key) {
    element_t * e = calloc(1,sizeof(element_t));
    e->ch1 = strdup(one_key);
    element_t *copy = one_list->head;
    if (copy == NULL) {
        one_list->head = e;

    }
    else {
        while (copy->suiv != NULL) {
            copy = copy->suiv;
        }
        copy->suiv = e;


    }
    
    
}



void element_print(element_t *one_element) {
    printf("%s\n",one_element->ch1);
    
}

void list_print(list_t *one_list) {
    if (!list_is_empty(one_list)) {
        element_t * elt_copy = one_list->head;
        while (elt_copy->suiv != NULL) {
            element_print(elt_copy);
            elt_copy = elt_copy->suiv;
        }
        element_print(elt_copy);
    }
    else {
        printf("[]\n");
    }
    
}

void list_remove_first(list_t* one_list) {
    if (list_is_empty(one_list)) {

    }
    else if (list_size(one_list) == 1) {
        free(one_list->head->ch1);
        free(one_list->head);
    }
    else {
        element_t *copyp = one_list->head;
        one_list->head = one_list->head->suiv;
        free(copyp->ch1);
        free(copyp);

        
    }
    
    

}

void list_remove(list_t* one_list, char* one_str) {
    if (list_is_empty(one_list)) {

    }
    else if (list_size(one_list) == 1) {
        if (strcmp(one_list->head->ch1,one_str) == 0) {
            free(one_list->head->ch1);
            free(one_list->head);
        }

    }
    else if (strcmp(one_list->head->ch1,one_str) == 0) {
        list_remove_first(one_list);

    }
    else {
        element_t *copyp = one_list->head->suiv;
        element_t *copy = one_list->head;
        while(copyp->suiv != NULL) {
            if (strcmp(copyp->ch1,one_str) == 0) {
                copy->suiv = copyp->suiv;
                free(copyp->ch1);
                free(copyp);
                copyp = copy->suiv;
            }
            else {
                copy = copyp;
                copyp = copyp->suiv;
            }
            
        }
        if (strcmp(copyp->ch1,one_str) == 0) {
            copy->suiv = copyp->suiv;
            free(copyp->ch1);
            free(copyp);
        }

    }
    
}

element_t * list_first(list_t* one_list) {
    if(!list_is_empty(one_list)) {
        return one_list->head;
    }
    return NULL;
}

element_t * list_last(list_t * one_list) {
    if (list_is_empty(one_list)) {
        return NULL;
    }
    else {
        element_t * elt_copy = one_list->head;
        while (elt_copy->suiv != NULL) {
            elt_copy = elt_copy->suiv;
        }
        return elt_copy;

    }
}

element_t * list_get(list_t* one_list, int index) {

    if (index == 0) {
        return list_first(one_list);
    }
    element_t * elt_copy = one_list->head;
    int i = 0;
    while ((i<index-1)&&(elt_copy->suiv != NULL)) {
        elt_copy = elt_copy->suiv;
        i++;
    }
    if (elt_copy->suiv == NULL) {
        return elt_copy;
    }
    else {
        return elt_copy->suiv;
    } 
}

bool list_contains(list_t *one_list, char *one_key) {
    if (list_is_empty(one_list)) {
        return false;
    }
    element_t * elt_copy = one_list->head;
    while (elt_copy->suiv != NULL) {
        if (strcmp(elt_copy->ch1,one_key) == 0) {
            return true;
        }
        elt_copy = elt_copy->suiv;
    }
    if (strcmp(elt_copy->ch1,one_key) == 0) {
        return true;
    }
    return false;
}

char *list_find(list_t *one_list, char *one_key) {
    if (list_is_empty(one_list)) {
        return NULL;
    }
    element_t * elt_copy = one_list->head;
    while (elt_copy->suiv != NULL) {
        if (strcmp(elt_copy->ch1,one_key) == 0) {
            return elt_copy->ch1;
        }
        elt_copy = elt_copy->suiv;
    }
    if (strcmp(elt_copy->ch1,one_key) == 0) {
        return elt_copy->ch1;
    }
    return NULL;


}
