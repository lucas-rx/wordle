#include <assert.h>

#include "list.h"
#include "table.h"
#include "dico.h"

int main() {

    printf("Début tests_dico.c\n");

    table_t* T = table_create(300);
    table_add(T, "epier");
    table_add(T, "berce");
    table_add(T, "caser");
    table_add(T, "enfui");
    table_add(T, "bicot");
    table_add(T, "bouse");
    table_add(T, "verni");
    table_add(T, "vibre");
    table_add(T, "essor");
    table_add(T, "boche");

    table_t* U = motsSuivantsPossibles("epier", "00000", 5, T);
    printf("Results here :\n");
    for (int i = 0; i < U -> size; i++) {
        if (!list_is_empty(U -> L[i])) {
            list_print(U -> L[i]);
        }
    }

    table_destroy(T);
    table_destroy(U);

    
    int longueurMots = 5; // Utiliser getLongueurMots() dans la version finale du solveur
    
    table_t* dico = dico_load("../dictionary/science_etonnante/mots_5_copie.txt");
    // En cas d'erreur ici : penser à vérifier l'adresse du fichier

    //Ici, toute une batterie de tests pour m'assurer que mes fonctions fonctionnent correctement.

    table_t* greens = table_create(TAILLE_DICO);
    findWordsGreen('k', 4, dico, greens);

    // Mettre la lettre (ici, 'k') en minuscule ou majuscule selon le dictionnaire chargé.
    // Les mots du dico sont en majuscules : mettre 'k' en majuscules
    // Les mots du dico sont en minuscules : mettre 'k' en minuscules
    // Une erreur surviendra si cette condition n'est pas respectée.

    //table_print(greens);
    printf("%d\n", taille_dico(greens));

    table_t* yellows = table_create(TAILLE_DICO);
    findWordsYellow('z', 4, longueurMots, dico, yellows);
    //table_print(yellows);
    printf("%d\n", taille_dico(yellows));

    table_t* greys = table_create(TAILLE_DICO);
    excludeWords('e', "10010", longueurMots, dico, greys);
    //table_print(greys);
    printf("%d\n", taille_dico(greys));

    table_t* greysWithIndex = table_create(TAILLE_DICO);
    excludeWordsWithIndex('z', 4, dico, greysWithIndex);
    //table_print(greysWithIndex);
    printf("%d\n", taille_dico(greysWithIndex));

    table_destroy(greens);
    table_destroy(yellows);
    table_destroy(greys);
    table_destroy(greysWithIndex);
    
    printf("\n\n");

    //table_t* table = table_create(300);
    //table_t* table = motsSuivantsPossibles("tarie", "12002", longueurMots, dico);
    //table_print(table);
    //printf("%d\n", taille_dico(table));
    //printf("1er mot : %s\n", getFirstWord(dico));

    //table_destroy(table);

    table_t* voidTable = table_create(300);
    table_destroy(voidTable);

    list_t* voidList = list_create();
    list_destroy(voidList);

    list_t* myList = list_create();
    list_append(myList, "10");
    list_append(myList, "20");
    list_append(myList, "30");
    list_append(myList, "40");
    list_append(myList, "50");
    list_append(myList, "60");
    list_append(myList, "70");
    list_append(myList, "80");
    list_append(myList, "90");
    list_print(myList);
    list_destroy(myList);

    assert(hasDuplicates("avait", 'a') == true);
    assert(hasDuplicates("avait", 'b') == false);
    assert(hasDuplicates("avait", 'v') == false);   

    // Calcul du meilleur mot d'ouverture
//   char* bestWord = getBestWord(dico, longueurMots);
//   printf("Meilleur mot dans dico_tests.c : %s\n", bestWord);

    /*
    C'est ici qu'il faut faire 'free(bestWord)', malheureusement cela engendre une erreur.
    A la place, j'ai initialisé le meilleur mot à "zzzzzzzzzzzz" (12 "z", vu que notre jeu prend
    en compte les mots de 4 à 12 lettres), et le problème est réglé.
    */

    table_destroy(dico);

    // Ces tests génèrent des leaks mais sont justes
    
    assert(strcmp(evaluation("examen", "utopie", 6), "100000") == 0);
    assert(strcmp(evaluation("gueule", "gorgeu", 6), "211000") == 0);
    assert(strcmp(evaluation("arizona", "zmpoozm", 7), "0001200") == 0);
    assert(strcmp(evaluation("rouleau", "boucler", 7), "1221100") == 0);
    assert(strcmp(evaluation("etablir", "etablri", 7), "2222211") == 0);
    assert(strcmp(evaluation("azerty", "azerty", 6), "222222") == 0);
    assert(strcmp(evaluation("azerty", "zertya", 6), "111111") == 0);
    assert(strcmp(evaluation("pipopipopi", "piperaient", 10), "2220010000") == 0);
    assert(strcmp(evaluation("accompagnatrice", "orthographieras", 15), "100101110011101") == 0);
    assert(strcmp(evaluation("accompagnatrice", "anticycloniques", 15), "211100001010101") == 0);
    assert(strcmp(evaluation("venue", "imbue", 5), "00022") == 0);
    assert(strcmp(evaluation("prieres", "traquer", 7), "0200120") == 0);
    assert(strcmp(evaluation("speed", "abide", 5), "00101") == 0);
    assert(strcmp(evaluation("speed", "erase", 5), "10110") == 0);
    assert(strcmp(evaluation("speed", "steal", 5), "20200") == 0);
    assert(strcmp(evaluation("speed", "crepe", 5), "01210") == 0);
    assert(strcmp(evaluation("about", "baton", 5), "11101") == 0);
    assert(strcmp(evaluation("prieres", "traquer", 7), "0200120") == 0);
    assert(strcmp(evaluation("vols", "viol", 4), "2110") == 0);
    assert(strcmp(evaluation("sains", "panas", 5), "02012") == 0);
    assert(strcmp(evaluation("smell", "slump", 5), "21010") == 0);
    assert(strcmp(evaluation("geole", "golee", 5), "21112") == 0);
    

    printf("Fin tests_dico.c\n");

    return EXIT_SUCCESS;
}