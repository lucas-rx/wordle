#include <stdlib.h>
#include <stdio.h>
#define TAILLE_MAX 10000 // Tableau de taille 1000
#include <stdbool.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include "dico.h"




char* base3(int b)
{
    char d[5];
    int i=0;
    int reste;
    for(int i=0; i<5; i++)
    {
        reste=b%3;
        b=floor(b/3);
        d[i]=reste+'0';
    }
    char *p=d;
    return p;
}

int occurence_comb0(char *mot, char a, char *comb){
    int c=0;
    for (int i=0; i<strlen(mot); i++){
        if (mot[i]==a){
            if (comb[i]==*"0") c++;
        }
    }
    return c;
}

int occurence_comb1(char *mot, char a, char *comb){
    int c=0;
    for (int i=0; i<strlen(mot); i++){
        if (mot[i]==a){
            if (comb[i]==*"1") c++;
        }
    }
    return c;
}

int occurence_comb01(char *mot, char a, char *comb){
    int c=0;
    for (int i=0; i<strlen(mot); i++){
        if (mot[i]==a){
            if (comb[i]==*"1" || comb[i]==*"0") c++;
        }
    }
    return c;
}

int occurence_comb_2(char *mot, char a, char *comb){
    int c=0;
    for (int i=0; i<strlen(mot); i++){
        if (mot[i]==a){
            if (comb[i]!=*"2") c++;
        }
    }
    return c;
}




bool test_mot(char *mot_pris, char *mot_test, char *comb){
    int N=strlen(mot_pris);
    for (int i=0; i<N; i++){
        if (mot_pris[i]==mot_test[i]){
            if (comb[i]!=*"2"){
                return 0;
            }
        }
        else{
            if (comb[i]==*"2"){
                return 0;
            }
            int c1=0;
            int c2=0;
            if (comb[i]==*"1"){
                c1=occurence_comb1(mot_pris,mot_pris[i],comb);
                c2=occurence_comb01(mot_test,mot_pris[i],comb);
                if (c1>c2){
                    return 0;
                }
            }
        
            else{
                if (occurence_comb01(mot_test,mot_pris[i],comb)!=0){
                    return 0;
                }
            }
        }
    }
    return 1;
}


float proba(char *mot, table_t *t, char *comb){
    float n=0;
    float d=0;
    int cb=0;
    // printf("%s\n",comb);
    for (int i=0; i<t->size; i++){
        list_t *l=t->L[i];
        element_t *current=l->head;
        // printf("%d\n",i);
        while (current!=NULL){
            // printf("%s",mot);
            // printf("%s",comb);

            if (test_mot(mot,current->ch1, comb)){
                n++;
            }
            d++;
            current=current->suiv;
        }
    }
    float r=n/d;
    return r;
    
}

float qt_info(table_t *T, char *mot){
    float info=0;
    for (int j=0; j<243; j++){
        char *comb=base3(j);
        // printf("%s",comb);
        // printf("i: %d\n",j);
        if (proba(mot,T,comb)!=0){
            info+=-proba(mot,T,comb)*log2(proba(mot,T,comb));
        }
    }
    // printf("%f\n",info);
    return log2(info);
}


void solveur(){
    
    char *f = "../dictionary/science_etonnante/mots_5.txt";
    table_t *T = dico_load(f);
    float M=7.010885;
    int cb;
    char *best="garee";
    int p=0;

    for (int i=0; i<T->size; i++){
        list_t *l=T->L[i];
        element_t *current=l->head;
        
        while (current!=NULL && p==0){
            if (current->ch1){
                printf("Longueur du mot:%ld lettres\n",strlen(current->ch1));
                p=1;
            }
            current=current->suiv;
        }
    }

    // printf("%f\n",proba("garee",T,"12000"));
    // printf("%d\n",test_mot("garee","gemis","21000"));


    // for (int i=0; i<T->size; i++){
    //     printf("%d\n",i);
    //     list_t *l=T->L[i];
    //     element_t *current=l->head;
    //     while (current!=NULL){
    //         if (qt_info(T,current->ch1)>M){
    //             M=qt_info(T,current->ch1);
    //             best=current->ch1;
    //         }
    //         current=current->suiv;
    //     }
    // }

    printf("Meilleurs mot:%s, info:%f\n",best,M);
    printf("Enter une combinaison: ");
    scanf("%d", &cb);
    char comb2[5];

    while(cb!=-1 && !table_empty(T)){

        for (int i=0; i<5; i++){
            comb2[i]=floor(cb/pow(10,4-i))+'0';
            cb=cb-floor(cb/pow(10,4-i))*pow(10,4-i);
        }


        table_t *temp=table_create(300);

        for (int i=0; i<T->size; i++){
            list_t *l=T->L[i];
            element_t *current=l->head;
            while (current!=NULL){
                // printf("%d,%s\n",test_mot(current->ch1,best,comb2),current->ch1);
                if (test_mot(best,current->ch1,comb2)==1){
                    table_add(temp,current->ch1);
                    // printf("test");
                }
                current=current->suiv;
            }
        }

        // printf("%d",test_mot("zozos",))

        table_destroy(T);
        table_t *T=table_create(300);

        for (int i=0; i<T->size; i++){
            list_t *l=temp->L[i];
            element_t *current=l->head;
            while (current!=NULL){
                table_add(T,current->ch1);
                current=current->suiv;
            }
        }

        table_print(T);
        table_destroy(temp);
        printf("%f\n",qt_info(T,"oxyde"));
        float M=0;
        char *best;
        for (int i=0; i<T->size; i++){
            list_t *l=T->L[i];
            element_t *current=l->head;
            while (current!=NULL){
                // printf("%f",qt_info(T,current->ch1));
                if (qt_info(T,current->ch1)>M){
                    M=qt_info(T,current->ch1);
                    best=current->ch1;
                }
                current=current->suiv;
            }
        }
        printf("%s",best);
        if (!best){
            cb=-1;
        }
        else{
            printf("Meilleurs mot:%s, info:%f\n",best,M);
            printf("Enter une combinaison: ");
            scanf("%d", &cb);
        }
        
    }


    table_destroy(T);
}



int main(){
    solveur();
    // table_t *l=table_create(10);
    // printf("%d",table_empty(l));
    // table_add(l,"armand");
    // table_add(l,"qdf");
    // table_add(l,"qsdf");
    // table_add(l,"azer");
    // table_add(l,"arxvcbmand");
    // table_add(l,"therz");
    // table_add(l,"vbcxvw");
    // table_add(l,"aer");
    // table_add(l,"wcxv");
    // table_add(l,"dfhsfdg");
    // printf("%d",table_empty(l));
    return 0;
}
