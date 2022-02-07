/* @Author BELLATIF Mamoune & BARKAOUI Meriam */
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "../include/table.h"  
#include "../include/dico_utils.h"

// Stocke dans table la valeur v sous la cle k 
void store(PTable_entry *table, int k, char v[]) { 
    PTable_entry te = (PTable_entry) malloc(sizeof(Table_entry)); 
    te->key = k; 
    strcpy(te->val, v); 
    te->next = *table; 
    *table = te; 
}  


// Retourne la valeur se trouvant dans table sous la cle k, ou NULL si la 
// cle n'existe pas 
char* lookup(PTable_entry table, int k) { 
    PTable_entry pe = table; 
    while (pe != NULL && pe->key != k) { 
        pe = pe->next; 
    } 
    if (pe != NULL) 
        return pe->val; 
    else 
        return NULL; 
} 
 
// Affiche le contenu de la table 
void display(PTable_entry table) { 
    /* display a été modifié pour ne pas afficher d'entré null */
    PTable_entry pe = table;
    int cpt=0; //compteur
    while (pe != NULL) { 
        if (pe->val!=NULL){
            printf("\033[1;31m");
            printf("%d -> %s\n", pe->key, pe->val); 
            printf("\033[0m");
            cpt++;
        }
        pe = pe->next; 
    } 
    if (cpt==0){
        rouge();
        printf("La table est vide!\n");
        couleur_reset();

    }
}

