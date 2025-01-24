#include <time.h>

#include "list.h"
#include "table.h"

#define TAILLE_MAX 10000

#ifndef __DICO_H__
#define __DICO_H__

table_t* dico_load(char* filename);

int getLongueurMots();

char* getVictoryPattern(int length);

int base3(char* pattern, int longueur);

int power(int base, int exposant);

double log2(double x);

char* evaluation(char* motJoueur, char* motSecret, int longueur);

double wordEntropy(char* word, int longueur, table_t* one_dico);

char* getBestWord(table_t* one_dico, int longueur);

table_t* motsSuivantsPossibles(char* bestWord, char* pattern, int longueurMots, table_t* one_dico);

void findWordsGreen(char letter, int index, table_t* one_dico, table_t* dico_to_fill);

void findWordsYellow(char letter, int banIndex, int longueurMots, table_t* one_dico, table_t* dico_to_fill);

void excludeWords(char letter, int longueurMots, table_t* one_dico, table_t* dico_to_fill);

void excludeWordsWithIndex(char letter, int index, table_t* one_dico, table_t* dico_to_fill);

bool hasDuplicates(char* word, char letter);

int longueur(char* pattern[], int N);

char* cop(char* pattern[], char* copie, int longueur_mot);

void patterns(list_t *L, int longueur_mot, char* pattern[], int rang);

bool good_place_in_chaine(char *m, char c, int index);

bool is_in_chaine_not_good_place(char *word, char c, int index,char *pattern);

void suitable(char *word, char *pattern,table_t * T, list_t *mots_pattern,int taille_list);

int taille_dico(table_t * T);

double entropy(char *word, table_t *T,list_t *patterns);

void add_list_table(list_t *L,table_t *T);

char * word_entropy_max(table_t *T,list_t *patterns);
bool validate(char *pattern);

#endif 

