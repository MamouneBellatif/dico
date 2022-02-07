/* @Author BELLATIF Mamoune & BARKAOUI Meriam */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "../include/table.h"
#include "../include/dico_vars.h"
#include "../include/dico_macros.h"
#include "../include/dico_utils.h"

/*Utilitaires diverses*/

int supprimer(PTable_entry *table, int k){
    /* Supprime l'entrée de clé k de la table table*/
    int flag=FAIL;
    PTable_entry tmp = *table;
    PTable_entry precede = NULL;
    if (tmp != NULL && tmp->key == k){ //cas ou on veux supprimer la tete
        *table = tmp->next;
        return DONE;
    }

    else{
        while (tmp != NULL && tmp->key != k){ //sinon on cherche k on recupere le noeud precedent S
            precede = tmp;
            tmp = tmp->next;
        }
        if(tmp!=NULL){ //si on a trouvé k
            precede->next=tmp->next; // on lis le suivant au precedent, k n'est plus lié
            flag=DONE;
        }
        else{
            flag=FAIL;
        }
        return flag;
    } 
}

int saisieEntier(char* message){ //fonction de saisie entier avec verification et nettoyage de stdin
    int key;
    printf("%s :", message);
    while (scanf("%d", &key) != 1) //verifier que c'est un entier
    {
        printf("Erreur: Veuillez rentrer un nombre entier...\n");
        int chr;
        do
        {
            chr = getchar(); //on évacue ce qui reste dans stdin
        } while ((chr != EOF) && (chr != '\n'));
        printf("%s : ", message);
    }
    return key;
}

void rouge() { //coloration du texte en rouge
    printf("\033[1;31m");
}
void couleur_reset () { //coloration du texte en blanc
    printf("\033[0m");
}
