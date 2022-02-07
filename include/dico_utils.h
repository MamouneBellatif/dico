/* @Author BELLATIF Mamoune & BARKAOUI Meriam */
#include "table.h"

#ifndef UTILS_H
#define UTILS_H


int supprimer(PTable_entry *table, int k);
int saisieEntier(char* message);//fonction de saisie entier avec verification et nettoyage de stdin
void rouge();
void couleur_reset ();

#endif