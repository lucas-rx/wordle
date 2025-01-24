#include "list.h"
#include "table.h"
#include "dico.h"



int main() {

    // Ici, une ébauche du solveur final.
    // Il fonctionne presque bien, desfois il y a encore des erreurs pendnat l'exécution (et des leaks je crois)

    printf("\n--+--+-- Solveur WORDLE --+--+--\n\n");
    printf("Commandes :\n\n");
    printf("Couleurs :\n");
    printf("0 : la lettre n'est pas dans le mot à deviner (gris)\n");
    printf("1 : la lettre est dans le mot à deviner, mais elle est MAL placée (orange)\n");
    printf("2 : la lettre est dans le mot à deviner et elle est BIEN placée (vert)\n\n");
    printf("Rentrez le mot que vous propose le solveur dans votre partie de WORDLE.\n");
    printf("Ensuite, rentrez dans le solveur la combinaison de couleurs que vous avez obtenue.\n\n");
    printf("-1 : arrêt du solveur\n\n");

    int longueurMots = getLongueurMots();

    printf("Pour cette partie, on joue avec des mots de %d lettres !\n\n", longueurMots);
    char* victoryPattern = getVictoryPattern(longueurMots);

    char filenameBegin[] = "../dictionary/science_etonnante/mots_";
    char filenameEnd[] = ".txt";
    char filename[200]; // Contient l'adresse du bon dictionnaire (du type "../dictionary/ods/dicoXlettres.txt")
    
    sprintf(filename, "%s%d%s", filenameBegin, longueurMots, filenameEnd);
    //printf("filename : %s\n\n", filename);
    // On "écrit" dans filename (le format est similaire à printf())
    // Ici, on concatène une string, un entier puis une string (%s%d%s) puis on les passe en argument de sprintf()

    //printf("Dico : %s\n", filename);

    table_t* dico = dico_load(filename);
    table_t* previousDico = dico;
    table_t* updatedDico = NULL;
    //int taille_full_dico = taille_dico(dico);
    char *bestWord = "zzzzzzzzzzzz";
    switch (longueurMots) {
        case 4:
            bestWord = "saie";
            break;
        case 5:
            bestWord = "tarie";
            break;
        case 6:
            bestWord = "taries";
            break;
        case 7:
            bestWord = "cariees";
            break;
        case 8:
            bestWord = "racinees";
            break;
        case 9:
            bestWord = "certaines";
            break;
        case 10:
            bestWord = "pertuisane";
            break;
        case 11:
            bestWord = "capitulions";
            break;
        case 12:
            bestWord = "reticulaires";
            break;
        default:
            bestWord = "tarie";

    }

    printf("\nMeilleur mot d'ouverture : %s\n",bestWord);
    
    // Respecter la casse = le mot est sensible au changement majuscule / minuscule ("tarie" =/= "TARIE")

    while(true) {
        // On propose au joueur le meilleur mot possible
        if (strcmp(bestWord, "zzzzzzzzzzzz") == 0) {
            break;
        }
        
        // Il répond dans la console : -1 pour arrêter, un pattern pour continuer (si pattern = que des "2" on arrête aussi)
        char* pattern = calloc(longueurMots + 20,sizeof(char)); // Le pattern tapé dans le terminal par le joueur
        printf("Combinaison de couleurs obtenue (-1 pour arrêter) : \n");
        unsigned long l = longueurMots;
        
        
        scanf("%s", pattern);

        
        

        //if (strcmp(pattern, "-1") == 0 || strcmp(pattern, victoryPattern) == 0) {
        if (strcmp(pattern, "-1") == 0) {
            // On arrête le solveur
            // -1 : arrêt par le joueur
            // Que des '2' : victoire
            free(pattern);
            
            break;
        }
        if (validate(pattern) == false) {
            free(pattern);
            printf("\n%s\n","Combinaison non valide !");
            continue;
        }
        
        if ((strlen(pattern) >= l + 1)||(strlen(pattern) < l)) {
            free(pattern);
            printf("\n%s\n","Combinaison non valide !");
            continue;
        }
        printf("\n");
        
        
        updatedDico = motsSuivantsPossibles(bestWord, pattern, longueurMots, previousDico);
        bestWord = getBestWord(updatedDico, longueurMots);

        /*
        list_t* updatedList = list_create();
        suitable(bestWord, pattern, previousDico, updatedList, 0);
        table_t* updatedDico = table_create(TAILLE_DICO);
        add_list_table(updatedList, updatedDico);
        bestWord = word_entropy_max(updatedDico, updatedList);
        list_destroy(updatedList);
        */
    
        free(pattern);
        
        //printf("Taille dico : %d\n", taille_dico(updatedDico));
        table_t* temp = previousDico;
        
        //printf("Length before permu | temp : %d, previous : %d, updated : %d\n", taille_dico(temp), taille_dico(previousDico), taille_dico(updatedDico));
        
        previousDico = updatedDico;
        //table_destroy(previousDico);
        if (temp != dico) {
            //printf("free(temp)\n");
            table_destroy(temp);
        }

       
        //printf("Length after permu | previous : %d, updated : %d\n", taille_dico(previousDico), taille_dico(updatedDico));
        //free(pattern);
        // On supprime du dictionnaire les mots ne pouvant plus être des réponses
        //Serge proposition : tu peux détruire la table et construire une nouvelle table puis 
        //utiliser la fonction add_list sur newList et la nouvelle table, add_list ajoute les élts
        //d'une liste dans une table 
    }
    // Libération de la mémoire
    printf("\n--+-- Arrêt du solveur --+--\n\n");
    
    free(victoryPattern); // victoryPattern a été alloué dynamiquement
    
    if (previousDico != NULL) {
        //printf("free(previousDico)\n");
        table_destroy(previousDico);
    }

    if (dico != NULL && dico != previousDico) {
        //printf("free(dico)\n");
        table_destroy(dico);
    }

    return EXIT_SUCCESS;
}

/*
int main() {
    
    assert(is_in_chaine_not_good_place("gagas",'g',0,"12000") == false);
    assert(good_place_in_chaine("radar",'r',4) == true);

    

    char *f = "../dictionary/science_etonnante/mots_5.txt";
    table_t *T = dico_load(f);
    //char *m = "garee";
    
    //for (int i = 0;i<T->size;i++) {
    //    printf("index:%d\n",i);
    //
    //    list_print(T->L[i]);
    //}
    
    list_t *L = list_create();
    char *m = "tarie";
    int rg = 0;
    char* pattern[10] = {};
    patterns(L,5,pattern,rg);

    //clock_t deb = clock();
    list_t *L1 = list_create();
    suitable(m,"12000",T,L1,rg);
    table_t *T1 = table_create(TAILLE_DICO);
    add_list_table(L1,T1);
    char * w = word_entropy_max(T1,L);

    
    list_t *L2 = list_create();
    suitable(w,"02101",T1,L2,0);
    table_t *T2 = table_create(TAILLE_DICO);
    add_list_table(L2,T2);
    char * p = word_entropy_max(T2,L);

    //clock_t fin = clock();
    //unsigned long diff = (fin-deb) / CLOCKS_PER_SEC;
    //printf("%ld\n",diff);
    //list_print(L);
    //printf("%d\n",list_size(L));
    //int h = hash("votes") % 300;
    //printf("%d\n",list_size(L));
    //list_print(L);
    
    list_destroy(L);
    list_destroy(L1);
    list_destroy(L2);
    table_destroy(T);
    table_destroy(T1);
    table_destroy(T2);
}*/
