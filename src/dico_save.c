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
#include "../include/dico_communication.h"
#include "../include/dico_save.h"
#include "../include/dico_node.h"

void sauvegarde(char nom_fichier[], PTable_entry tete, int node_index){
    /*Sauvegarde la liste chainé tete  du node node_index dans un fichier nom_fichier*/
    //il faut sychroniser (de 0 à n)la sauvegarde sinon le node 0 écrase les noeuds qui ont sauvegardé avant
    syncNode(node_index, IN_PROGRESS, EXIT); // debut de synchronisation inter-node

    PTable_entry temp = tete;
    
    FILE* fichier; //descripteur du fichier de sauvegarde

    
    //ils doivent sauvegarder tour a tour
    if(node_index==0){ //si c'est le premier node qui fait a sauvegarde, on efface la sauvegarde précedente
        fichier = fopen (nom_fichier, "w");
    }
    else{ //si on c'est un autre node qui effecture la sauvegarde, on concatene
        fichier = fopen (nom_fichier, "a");
    }
    if (fichier == NULL)
    {
        fprintf(stderr, "\nErreur ouverture fichier\n");
        exit (1);
    }
    
    // On écrit dans le fichier une par une toutes les entré de la table
    while(temp!=NULL){
        if(temp->val!=NULL){
             fwrite(temp, sizeof(struct _Table_entry), 1, fichier);
             temp = temp->next;
        }
 
    }
    
    int flag;
    if(fwrite != 0){
        // printf("sauvegarde...\n");
        flag=DONE;
    }
       else{
           printf("Error sauvegarde\n");
        flag=FAIL;
    }
    
    fclose(fichier);
    syncNode(node_index, flag, EXIT); //fin de synchronisation inter-node
    
}
PTable_entry chargerSauvegarde(char nom_fichier[], int node_index){ 
    /* Charge depuis le fichier nom_ficher la liste chainé dans le node node_index*/
    PTable_entry temp = (PTable_entry)malloc(sizeof(struct _Table_entry)); //tabe temportaire
    PTable_entry tete =NULL; // premier element
    PTable_entry last=NULL; // dernier element
    
    FILE* fichier;
    fichier = fopen (nom_fichier, "ab+"); //on ouvre le fichier de sauvegarde, si il n'existe pas, on en creer un nouveau

    if (fichier == NULL){     //cas erreur
        fprintf(stderr, "\nErreur ouverture fichier'\n");
        exit (1);
    }
    while(fread(temp, sizeof(struct _Table_entry), 1, fichier)){ //on lit les  entrés de la liste chainé sauvegardé dans le fichier
        if (checkNode(temp->key, node_index) == 0){
             //on ne charge que les entrées dont s'occupe le node courant, on evite qu'un node charge une clé qui ne lui correspond pas
            if (tete == NULL){ //intitalisation, premiere iteration du chargement
                last = (PTable_entry)malloc(sizeof(struct _Table_entry));
                tete = last;
            }

            else{
                last->next = (PTable_entry)malloc(sizeof(struct _Table_entry)); //on affecte le pointeur vers un nouvel element de la liste
                last = last->next;
            }
            last->key = temp->key; //on charge les valeur lues
            strcpy(last->val, temp->val); //pareil
            last->next = NULL;
        }
    }
    fclose(fichier);
    return tete;
}