#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <string.h>

#include "list.h"
#include "table.h"

#define TAILLE_MAX 10000 // Tableau de taille 10 000

#ifndef __SOLVEUR_H__
#define __SOLVEUR_H__

int getLongueurMots();

char* base3(int b);

int occurence_comb0(char *mot, char a, char *comb);

int occurence_comb(char *mot, char a, char *comb);

bool test_mot(char *mot_pris, char *mot_test, char *comb);

double proba(char *mot, table_t *t, char *comb);

double qt_info(table_t *T, char *mot);

void solveur();

#endif
