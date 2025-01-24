#include "dico.h"

table_t* dico_load(char* filename) { // Charge le dictionnaire 'filename' dans une table de hachage
    printf("Chargement du dictionnaire...\n");
    table_t *T = table_create(TAILLE_DICO);
    FILE *f = fopen(filename,"r");
    while (!feof(f)) {
        char buffer[20];
        fgets(buffer,20,f);
        char res[20];
        sscanf(buffer,"%[^\n]",res);
        table_add(T,res);
    }
    fclose(f);
    printf("Dictionnaire chargé !\n");
    printf("Nombre d'alvéoles de la table : %d\n", T -> size);
    printf("Taille du dictionnaire : %d\n", taille_dico(T));
    return T;
}

int getLongueurMots() {
    
    // Récupère la longueur des mots du dictionnaire
    
    FILE* wsolf = fopen("./wsolf.txt", "r");
    
    int longueurMots = 0;

    if (wsolf != NULL) {
        char chaine[5];
        fgets(chaine, 5, wsolf);
        int l;
        sscanf(chaine,"%d\n",&l);
        longueurMots = l;
        
    }
    fclose(wsolf);
    //printf("%d\n",longueurMots);
    return longueurMots;
}

// NON UTILISEE
char* getVictoryPattern(int length) {

    // 4 -> "2222", 5 -> "22222" etc. (toutes les lettres sont bien placées)
    // Il faut free pattern, sinon leak
    // 'length' est la longueur du pattern

    char* pattern = calloc(length + 1, sizeof(char));
    for (int i = 0; i < length; i++) {
        pattern[i] = '2';
    }
    pattern[length] = '\0'; // Pour indiquer la fin de la chaîne
    return pattern;
}

int base3(char* pattern, int longueur) {
    
    // Convertit une string en base 3 enn un nombre en base 10
    // Exemple : "12002" -> 137
    // 'longueur' est la longueur des mots
    
    int inBase3 = 0;
    for (int i = 0; i < longueur; i++) {
        if (pattern[i] == 49) { // Code ASCII de 1 : 49
            inBase3 += 1 * power(3, longueur - 1 - i);
        }
        else if (pattern[i] == 50) { // Code ASCII de 2 : 50
            inBase3 += 2 * power(3, longueur - 1 - i);
        }
    }
    return inBase3;
}

double log2(double x) {
    if (x > 0.0) {
        return log(x) / log(2);
    } else {
        return 0.0;
    }
}

char* evaluation(char* motJoueur, char* motSecret, int longueur) {
    
    // IL FAUT FREE RESULTAT, SINON LEAK
    // Retourne la combinaison de couleurs telle que le joueut joue le mot 'motJoueur'
    // et le mot à deviner est 'motSecret'
    
    char* motJoueurCopie = calloc(longueur + 1, sizeof(char));
    char* motSecretCopie = calloc(longueur + 1, sizeof(char));
    char* resultat = calloc(longueur + 1, sizeof(char)); // Le dernier caractère vaut '\0' (valeur initialisée par défaut dans toutes les cases d'un tableau)

    for (int m = 0; m < longueur; m++) {
        motJoueurCopie[m] = motJoueur[m];
        motSecretCopie[m] = motSecret[m];
        resultat[m] = '0';
    }

    for (int i = 0; i < longueur; i++) {
        if (motJoueurCopie[i] == motSecretCopie[i]) {
            resultat[i] = '2';
            motJoueurCopie[i] = '#';
            motSecretCopie[i] = '#';
        }
    }

    for (int i = 0; i < longueur; i++) {
        for (int j = 0; j < longueur; j++) {
            if (motJoueurCopie[i] != '#' && i != j && motJoueurCopie[i] == motSecretCopie[j]) {
                resultat[i] = '1';
                motJoueurCopie[i] = '#';
                motSecretCopie[j] = '#';
            }
        }
    }

    free(motJoueurCopie);
    free(motSecretCopie);
    //printf("Résultat : %s\n", resultat);
    return resultat;
}

double wordEntropy(char* word, int longueur, table_t* one_dico) {

    // Calcule l'entropie du mot 'word' inséré dans la table 'one_dico'
    // 'longueur' est la longueur des mots

    double entropy = 0.0;
    int taille_one_dico = taille_dico(one_dico);
    int* patternsRepartition = calloc(power(3, longueur), sizeof(int));

    for (int i = 0; i < one_dico -> size; i++) {
        element_t* current = one_dico -> L[i] -> head;
        while (current != NULL) {
            for (int j = 0; j < list_size(one_dico -> L[i]); j++) {
                //printf("word : %s, current -> ch1 : %s\n", word, current -> ch1);
                char* pattern = evaluation(word, current -> ch1, longueur);
                int patternInBase3 = base3(pattern, longueur);
                //printf("Pattern obtenu : %s | En base 3 : %d\n", pattern, patternInBase3);
                patternsRepartition[patternInBase3]++;
                //printf("Après incr' : %d\n", patternsRepartition[patternInBase3]);
                //printf("escape\n");
                current = current -> suiv;
                free(pattern);
            }
        }
        
    }

    for (int i = 0; i < power(3, longueur); i++) {
        double proba = (double) patternsRepartition[i] / taille_one_dico;
        //printf("Dans patternsRepart : %d, proba : %lf\n", patternsRepartition[i], proba);
        //printf("Taille dico : %d\n", taille_one_dico);
        entropy += -proba * log2(proba);
    }

    free(patternsRepartition);
    return entropy;
}


char* getBestWord(table_t* one_dico, int longueur) { // Il faut free bestWord

    // Retourne le mot du dictionnaire 'one_dico' avec la meilleure entropie
    // 'longueur' est la longueur des mots

    clock_t debut = clock();

    char* bestWord = "zzzzzzzzzzzz"; // Mot par défaut
    /*
    Quand je fais une allocation dynamique avec calloc :
    char* bestWord = calloc(longueur + 1, sizeof(char));
    il faut penser à free ensuite.

    Malgré ça, quand je veux free 'bestWord' dans le main après l'exécution du programme,
    j'ai une erreur : attempting double-free.
    Je n'ai aucune idée de comment résoudre l'erreur. -Lucas
    */

    //char* bestWord = one_dico -> L[0] -> head -> ch1;
    //printf("Best init word : %s\n", bestWord);
    double bestEntropy = -1.0;

    for (int i = 0; i < one_dico -> size; i++) {
        element_t* current = one_dico -> L[i] -> head;
        while (current != NULL) {
            //printf("Current word : %s\n", current -> ch1);
            double currentWordEntropy = wordEntropy(current -> ch1, longueur, one_dico);
            if (currentWordEntropy > bestEntropy) {
                bestWord = current -> ch1;
                bestEntropy = currentWordEntropy;
                //printf("New best word : %s | entropy : %lf bits\n", bestWord, bestEntropy);
            }
            current = current -> suiv;
        }
        
    }

    clock_t fin = clock();
    unsigned long diff = (fin - debut) / CLOCKS_PER_SEC;
    printf("Temps écoulé : %ld secondes\n", diff); // Affichage du temps de traitement en secondes
    if (strcmp(bestWord, "zzzzzzzzzzzz") == 0) {
        printf("Erreurs de saisie : échec de résolution\n");
        return bestWord;
    }
    printf("Meilleur mot : %s avec %lf bits d'information\n", bestWord, bestEntropy);
    printf("Réponses restantes possibles : %d\n", taille_dico(one_dico));
    return bestWord;
}

table_t* motsSuivantsPossibles(char* bestWord, char* pattern, int longueurMots, table_t* one_dico) {
    
    // Retourne dans une table de hachage l'ensemble des mots pouvant encore être des solutions
    // 'bestWord' : le meilleur mot précédent OU le meilleur mot d'ouverture (ex : tarie) selon l'avancement du solveur
    // 'pattern' est une combinaison de couleurs (ex : "12002")
    // 'longueurMots' est la longueur des mots
    // 'one_dico' : dictionnaire des mots pouvant encore être des solutions (on a supprimé les mots ne pouvant plus être
    // des solutions en fonction des combinaisons de couleurs rentrées par le joueur)

    table_t* oldDico = one_dico; // Dictionnaire "précédent" : parmi les mots de ce dictionnaire, on s'apprête
    // à supprimer les mots ne pouvant plus être des solutions

    table_t* newDico = table_create(TAILLE_DICO); // Dictionnaire mis à jour : on a retiré certains mots
    //printf("INIT | Length old : %d, new : %d\n\n", taille_dico(oldDico), taille_dico(newDico));
    
    for (int i = 0; i < longueurMots; i++) { // Détection lettres bien placées
        if (pattern[i] == '2') {
            findWordsGreen(bestWord[i], i, oldDico, newDico);
            table_t* temp = oldDico;
            //printf("Length before permu | temp : %d, old : %d, new : %d\n", taille_dico(temp), taille_dico(oldDico), taille_dico(newDico));
            oldDico = newDico;
            newDico = table_create(TAILLE_DICO);
            //printf("Length after permu | temp : %d, old : %d, new : %d\n", taille_dico(temp), taille_dico(oldDico), taille_dico(newDico));
            if (temp != one_dico) { // On ne supprime pas le dictionnaire original contenant tous les mots de X lettres, sinon erreur
                //printf("table_destroy(temp)\n");
                table_destroy(temp);
            }
        }
    }

    //printf("Green : %d\n\n", taille_dico(oldDico));

    for (int i = 0; i < longueurMots; i++) { // Détection lettres mal placées
        if (pattern[i] == '1') {
            findWordsYellow(bestWord[i], i, longueurMots, oldDico, newDico);
            table_t* temp = oldDico;
            //printf("Length before permu | temp : %d, old : %d, new : %d\n", taille_dico(temp), taille_dico(oldDico), taille_dico(newDico));
            oldDico = newDico;
            newDico = table_create(TAILLE_DICO);
            //printf("Length after permu | temp : %d, old : %d, new : %d\n", taille_dico(temp), taille_dico(oldDico), taille_dico(newDico));
            if (temp != one_dico) {
                //printf("table_destroy(temp)\n");
                table_destroy(temp);
            }
        }
    }

    //printf("Yellow : %d\n\n", taille_dico(oldDico));

    for (int i = 0; i < longueurMots; i++) { // Détection lettres impossibles
        if (pattern[i] == '0') {
            if (!hasDuplicates(bestWord, bestWord[i])) {
                excludeWords(bestWord[i], longueurMots, oldDico, newDico);
            } else {
                excludeWordsWithIndex(bestWord[i], i, oldDico, newDico);
            }
            table_t* temp = oldDico;
            //printf("Length before permu | temp : %d, old : %d, new : %d\n", taille_dico(temp), taille_dico(oldDico), taille_dico(newDico));
            oldDico = newDico;
            newDico = table_create(TAILLE_DICO);
            //printf("Length after permu | temp : %d, old : %d, new : %d\n", taille_dico(temp), taille_dico(oldDico), taille_dico(newDico));
            if (temp != one_dico) {
                //printf("table_destroy(temp)\n");
                table_destroy(temp);
            }
        }
    }

    //printf("End : %d\n", taille_dico(oldDico));

    table_destroy(newDico);
    //printf("table_destroy(newDico)\n");
    return oldDico;
}

void findWordsGreen(char letter, int index, table_t* one_dico, table_t* dico_to_fill) {

    // Remplit la table de hachage 'dico_to_fill' avec tous les mots de 'one_dico' ayant la lettre 'letter' à l'indice 'index'.
    // 'longueurMots' est la longueur des mots
    
    for (int i = 0; i < one_dico -> size; i++) {
        element_t* current = one_dico -> L[i] -> head; // Premier élément de la liste n°i
        while (current != NULL) {
            if (current -> ch1[index] == letter) {
                table_add(dico_to_fill, current -> ch1);
            }
            current = current -> suiv;
        }
        
    }
}

void findWordsYellow(char letter, int banIndex, int longueurMots, table_t* one_dico, table_t* dico_to_fill) {
    
    // Remplit la table de hachage 'dico_to_fill' avec tous les mots de 'one_dico' contenant la lettre 'letter', 
    // sauf si 'letter' est à l'indice 'banIndex'.
    // (puisqu'on sait par définition de la couleur jaune de Wordle que la lettre est mal placée)
    // 'longueurMots' est la longueur des mots
    
    for (int i = 0; i < one_dico -> size; i++) {
        element_t* current = one_dico -> L[i] -> head; // Premier élément de la liste n°i
        while (current != NULL) {
            for (int j = 0; j < longueurMots; j++) {
                if (current -> ch1[banIndex] != letter && current -> ch1[j] == letter) {
                    table_add(dico_to_fill, current -> ch1);
                }
            }
            current = current -> suiv;
        }
        
    }
}

void excludeWords(char letter, int longueurMots, table_t* one_dico, table_t* dico_to_fill) {

    // Remplit la table de hachage 'dico_to_fill' avec tous les mots de 'one_dico' ne contenant pas la lettre 'letter'.
    // 'longueurMots' est la longueur des mots

    for (int i = 0; i < one_dico -> size; i++) {
        element_t* current = one_dico -> L[i] -> head; // Premier élément de la liste n°i
        while (current != NULL) {
            for (int j = 0; j < longueurMots; j++) {
                if (current -> ch1[j] == letter) {
                    break;
                }
                else if (j == longueurMots - 1) {
                    table_add(dico_to_fill, current -> ch1);
                }
            }
            current = current -> suiv;
        }
        
    }
}

void excludeWordsWithIndex(char letter, int index, table_t* one_dico, table_t* dico_to_fill) {

    // Permet de gérer les lettres en plusieurs exemplaires dans les mots,
    // notamment si leur couleur est différente
    // Remplit la table de hachage 'dico_to_fill' avec tous les mots de 'one_dico' n'ayant pas la lettre 'letter' à la position 'index'.
    // 'longueurMots' est la longueur des mots

    for (int i = 0; i < one_dico -> size; i++) {
        element_t* current = one_dico -> L[i] -> head;
        while (current != NULL) {
            if (current -> ch1[index] != letter) {
                table_add(dico_to_fill, current -> ch1);
            }
            current = current -> suiv;
        }
        
    }
}

bool hasDuplicates(char* word, char letter) {

    // Vérifie si le mot 'word' contient plusieurs exemplaires de la legtre 'letter' ou non

    int nbDuplicates = 0;
    int i = 0;
    while(word[i]) {
        if (word[i] == letter) {
            nbDuplicates++;
        }
        i++;
    }
    return nbDuplicates >= 2;
}



int longueur(char* pattern[], int N) {
    if (pattern[0] == NULL) {
        return 0;
    }
    
    int j = 0;
    for (int i = 0;i<N;i++) {
        if(pattern[i]) {
            j++;
        }
    }
    return j;
}

char * cop(char* pattern[],char* copie,int longueur_mot) {
    for (int i = 0;i<longueur_mot;i++) {
        strcat(copie,pattern[i]);
    }
    return copie;

}

void patterns(list_t *L,int longueur_mot,char* pattern[],int  rang) {
    if (rang >= longueur_mot) {
        char *copy = malloc((longueur_mot+5)*sizeof(char));
        strcpy(copy,"");
        copy = cop(pattern,copy,longueur_mot);
        //printf("%s\n",copy);
        list_append(L,copy);
        free(copy);
    }
    else {
        for(int i = 0;i<3;i++) {
            char num[3];
            sprintf(num,"%d",i);
            pattern[rang] = num;
            //printf("%ld\n",rang);
            patterns(L,longueur_mot,pattern,rang + 1);
            pattern[rang] = NULL;
        }

    }
    
}
bool validate(char *pattern) {
    for(unsigned long i = 0;i<strlen(pattern);i++) {
        if ((pattern[i] == '0')||(pattern[i] == '1')||(pattern[i] == '2')) {
            continue;
        }
        else {
            return false;
        }
    }
    return true;
}

bool good_place_in_chaine(char *m, char c, int index) {
    if (c == m[index]) {
        return true;
    }
    return false;
}

bool is_in_chaine_not_good_place(char *word, char c,int index,char *pattern) {
    int N = strlen(word);
    for (int i = 0;i<N;i++) {
        if ((i != index)&&(word[i] == c)&&(pattern[i] != '2')) {
            if (good_place_in_chaine(word,c,index) == false) {
                return true; 
            }
              
        }
    }
    return false;
}


void suitable(char *word, char *pattern,table_t * T, list_t *mots_pattern,int taille_list) {
    int N = strlen(word);
    if (pattern[0] == '2') {
        if (list_is_empty(mots_pattern)) {
            for (int j = 0;j<T->size;j++) {    
                list_t *M = T->L[j];
                if (!list_is_empty(M)) {
                    element_t * elt_copy = M->head;
                    while (elt_copy != NULL) {
                        if (good_place_in_chaine(elt_copy->ch1,word[0],0)) {
                            //printf("%s\n",elt_copy->ch1);
                            list_append(mots_pattern,elt_copy->ch1);
                        }
                        elt_copy = elt_copy->suiv;
                    }    
                }
            }
        }
    }
    if (pattern[0] == '1') {
        if (list_is_empty(mots_pattern)) {
            for (int j = 0;j<T->size;j++) { 
                list_t *M = T->L[j];
                if (!list_is_empty(M)) {
                    element_t * elt_copy = M->head;    
                    while (elt_copy != NULL) {
                        if (is_in_chaine_not_good_place(elt_copy->ch1,word[0],0,pattern)) {     
                            list_append(mots_pattern,elt_copy->ch1);         
                        }
                        elt_copy = elt_copy->suiv;
                    }
                }
            }       
        }
    }
    if (pattern[0] == '0') {
        for (int j = 0;j<T->size;j++) {    
            list_t *M = T->L[j];     
            if (!list_is_empty(M)) {
                element_t * elt_copy = M->head;
                while (elt_copy != NULL) {
                    if (is_in_chaine_not_good_place(elt_copy->ch1,word[0],0,pattern) == false) {
                        if (good_place_in_chaine(elt_copy->ch1,word[0],0) == false) {
                            list_append(mots_pattern,elt_copy->ch1);

                        } 
                    }
                    elt_copy = elt_copy->suiv;
                }        
            }
        }
    }
    if (!list_is_empty(mots_pattern)) {
        for (int i = 1;i<N;i++) {
            taille_list = list_size(mots_pattern);
            if (pattern[i] == '2') {
                list_t *M = mots_pattern;
                element_t * elt_copy = M->head;
                int k = 0;
                while ((k < taille_list)&&(elt_copy != NULL)) {
                    if (good_place_in_chaine(elt_copy->ch1,word[i],i)) {
                        //printf("\n");
                        //printf("%s\n",elt_copy->ch1);
                        list_append(mots_pattern,elt_copy->ch1);       
                    }
                    k++;
                    elt_copy = elt_copy->suiv;
                }
                list_destroy_index(mots_pattern,k);
                if (list_is_empty(mots_pattern)) {
                    break;
                }


            }
            else if (pattern[i] == '1') {
                list_t *M = mots_pattern;
                element_t * elt_copy = M->head;
                int k = 0;
                while ((k < taille_list)&&(elt_copy != NULL)) {
                    if (is_in_chaine_not_good_place(elt_copy->ch1,word[i],i,pattern)) {
                        list_append(mots_pattern,elt_copy->ch1);         
                    }
                    k++;
                    elt_copy = elt_copy->suiv;
                }
                list_destroy_index(mots_pattern,k);
                if (list_is_empty(mots_pattern)) {
                    break;
                }
            }
            else {
                list_t *M = mots_pattern;
                element_t * elt_copy = M->head;
                int k = 0;
                while ((k < taille_list)&&(elt_copy != NULL)) {
                    if (is_in_chaine_not_good_place(elt_copy->ch1,word[i],i,pattern) == false) {
                        if (good_place_in_chaine(elt_copy->ch1,word[i],i) == false) {
                            list_append(mots_pattern,elt_copy->ch1);
                        }                  
                    }
                    k++;
                    elt_copy = elt_copy->suiv;
                }
                list_destroy_index(mots_pattern,k);
                if (list_is_empty(mots_pattern)) {
                    break;
                }

            }

        }
        
    }
    
}

int taille_dico(table_t * T) {
    int taille = 0;
    for (int j = 0;j<T->size;j++) {    
        list_t *M = T->L[j];
        taille += list_size(M);
    }
    return taille;
}


double entropy(char *word, table_t *T,list_t *patterns) {
    double tl = taille_dico(T);
    //printf("%d\n", tl);
    element_t *copy = patterns->head;
    double E = 0;
    while(copy != NULL) {
        int t = 0;
        list_t *mots_p = list_create();
        suitable(word,copy->ch1,T,mots_p,t);
        double proba = (list_size(mots_p)) / tl;
        //printf("%s %d %s %lf %f\n",copy->ch1,list_size(mots_p),word,proba,tl);
        if (proba != 0) {
            E +=  (- proba) * (log(proba) / log(2));
        }
        list_destroy(mots_p);
         
        copy = copy->suiv;
    }
    //printf("%s %lf bits\n",word,E);
    return E;
}

void add_list_table(list_t *L,table_t *T) {
    if (!list_is_empty(L)) {
        element_t *copy = L->head;
        while(copy != NULL) {
            table_add(T,copy->ch1);
            copy = copy->suiv;
        }
    }
}


char * word_entropy_max(table_t *T,list_t *patterns) {
    printf("Calcul du meilleur mot possible...\n");
    double Emax = 0;
    char *m = "mot";
    for (int j = 0;j<T->size;j++) {    
        list_t *M = T->L[j];
        if (!list_is_empty(M)) {
            element_t *copy = M->head;
            while(copy != NULL) {
                double E = entropy(copy->ch1,T,patterns);
                if (E >= Emax) {
                    
                    Emax = E;
                    m = copy->ch1;
                }
                copy = copy->suiv;
            }
        }
    }
    printf("Le meilleur mot est : %s avec une entropie de %lf bits d'information\n", m, Emax);
    return m;

}






