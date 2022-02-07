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
#include "../include/dico_pipes.h"

void init(int nc){ 
    /*Description: Cette fonction initialise et affectea partir du nombre du processur choisi (n) les
            tubes et leurs descripteurs de fichier*/

    node_count = nc;
    pipe(f_master);
    f_node = malloc(node_count * sizeof(*f_node)); //on crée N descripteurs
    for (int i = 0; i < node_count; i++)
    {
        pipe(f_node[i]); 
    }
}

void fermerDescripteurs(int node_index){ //@ferme en fin dexecution les descripteurs utilsié par le node node_index ou controler -nodeindex ==-1)
    switch (node_index)
    {
    case MASTER://Si i==--1 alors on s'occupe du père
        close(f_master[0]); //on ferme lecture père (fin)
        for (int j = 0; j < node_count; j++)
        {
           close(f_node[j][1]); //on ferme ecriture  des fils (fin)
        }
        break;
    
    default: //sinon on s'occupe du node i
        close(f_node[node_index][1]); //fermeture node i ecriture
        if (node_index!=0){ 
            close(f_node[node_index-1][0]); //fermeture lecture cas node!=0
        }
        else {
            close(f_node[node_count-1][0]); //fermeture lecture cas node 0
        }
        close(f_master[1]); //on ferme l'ecriture de père
        break;
    }  
}

void ouvrirDescripteurs(int node_index){ //ferme touts les descripteur du node node_index ou controller(nodeindex -1-1) sauf ceux qu'on souhaite utiliser
    switch (node_index){
    case MASTER: //cas processus père (controller)
        close(f_master[1]); //on ferme le descripteur en ecriture inutilisé
        for (int j = 0; j < node_count; j++){
            close(f_node[j][0]); //on ferme lecture des processus fils intuilisé
        }
        break;

    default:// cas procesur fils node
        close(f_master[0]); //on ferme le descripteur en lecture intuilisé
        close(f_node[node_index][0]); //on ferme le descripteur en lecture du node courant
        for (int j = 0; j < node_count; j++) //on parcours tous les descripteurs fils pour les comparer au descripteur courant
        {
            if (node_index != j) // descripteur autre que ceux du node courant
            {
                if (j == node_index - 1){
                    close(f_node[j][1]); //on ferme le descripteur précedent en ecriture
                }
                else{
                    if (node_index == 0 && j == node_count - 1){ //on fait l'exception pour le premier node()
                        close(f_node[j][1]);//on laisee ouvert dans ce cas précis pour node 0 puisse lire sur le dernier tube
                    }
                    else{
                        close(f_node[j][0]); //on ferme tous les autres descripteurs 
                        close(f_node[j][1]);
                    }
                }
            }
        }
         break;
    }    
}