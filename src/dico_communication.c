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
#include "../include/dico_utils.h"

/*Foctions qui gèrent la communication, la synchronisation et l'execution des requettes*/

void readRequest(int node_index,int *cmd, char *value,int *key, int *flag){ //lit la requête et l'interpète et renvoie aussi le buffer original
    char buffer[100]; //tempon message
    switch (node_index)
    {
    case MASTER: //cas père
         //le père lis un message reçu
        read(f_master[0], buffer, 100*sizeof(char));
        break;
    case 0: //cas premier node
        read(f_node[node_count - 1][0], buffer, 100*sizeof(char));
        //printf("première lecture. \n");
        break;
    default: //cas nodes mis a part le premier
        read(f_node[node_index - 1][0], buffer, 100*sizeof(char));
        // printf("lecture tube. \n");
        break;
    }
    sscanf(buffer, "%d %d %d %s", flag, cmd, key, value); //on deconstruit la requete
}

void sendRequest(int node_index, int cmd, char *value, int key, int flag){//status flag (0, non traité envoyer au prochai noeud, 1 done: envoyer au père | IN_PROGRESS | DONE | FAIL
   
    /* Prend en paramètre l'indice du node a l'origine de la requeter
    (MASTER si evoyé par le controler), la commande, la valeure, 
    la clée et le flag d'état de la requet;  construit la requete dans un buffer
    et l'envoie au processus correspondant selon la situation*/

    char buffer[100]; //tempon message
    sprintf(buffer,"%d %d %d %s",flag, cmd, key, value);
    switch (node_index)
    {
    case MASTER:
        if(cmd==EXIT || cmd==DUMP){ //si on quitte le programme on qu'on fait un dump on ecrit a tous les nodes
            for (int j = 0; j < node_count; j++) //envoie a tous les nodes
            {
                write(f_node[j][1], buffer, 100*sizeof(char)); 
            }
            
        }
        else{
            write(f_node[node_count-1][1], buffer, 100*sizeof(char)); //envoie au permier node
        }
        break;
    default:
        if (flag==DONE || flag==FAIL || node_index==node_count-1){//le dernier node ecris au père que la requete a echouée si aucun noeud ne prend la requette
            sprintf(buffer,"%d %d %d %s",flag, cmd, node_index, value); //on reconstruit la requeete pour quon renvoie node_index a la place de key pour informer le pere
            write(f_master[1], buffer, 100*sizeof(char));  //on répond au controller
        }
        else{
            write(f_node[node_index][1], buffer, 100*sizeof(char)); //on renvoie la requette au noeud suivant si le current node n'a pas executé la requête
        }
        break;
    }
} 

void syncNode(int node_index, int flag, int cmd){ //
    /* synchronise les nodes pour le dump et la sauvegarde,
     a apppeler au debut et a la fin de l'execution, node_index est le node qui a appelé la f,ction,
      flag pour savoir si on est au debut ou a la fin de la sync, et cmd la commande a synchroniser*/

    char *token = "done"; //jeton pour la synchronisation
    if (node_index != 0 && flag==IN_PROGRESS){ //attend que le node précedent aie fini d'executer
        read(f_node[node_index - 1][0], token, 5*sizeof(char)); 
    }
    else if (node_index != node_count - 1 && flag==DONE){ //indique au noeud suivant qu'il a fini l'exection
        write(f_node[node_index][1], token, 5*sizeof(char));
    }
    if(node_index==node_count-1 && flag==DONE) { // le dernier noeud informe le père que l'execution est finie, le père peut reprednre
        sendRequest(node_index, cmd, "null", 0, DONE);
    }
}

