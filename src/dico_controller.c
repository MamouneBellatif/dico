/* @Author BELLATIF Mamoune & BARKAOUI Meriam */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "../include/dico_vars.h"
#include "../include/dico_macros.h"
#include "../include/dico_utils.h"
#include "../include/dico_pipes.h"
#include "../include/dico_communication.h"
#include "../include/dico_controller.h"
/*Foctions qui gèrent la communication, la synchronisation et l'execution des requettes*/


void controller(int *proc_id){
    int cmd; //commande a executer
    int flag; //flag etat de la requete (IN_PROGRESS, DONE, FAIL) voir macro
    int key;//clé de la valeure
    char value[100]; //valeure a stocker
    strcpy(value,"null");  //on met une valeure nulle au cas ou lutilisateur decide de lancer en premier une commande qui ne necessite pas de valeure
    ouvrirDescripteurs(MASTER); //on ferme les descripteur inutilisés
    do
    {
        cmd=saisieEntier("\nSaisir commande (0 = save and exit, 1 = set, 2 = lookup, 3 = dump, 4 = delete)"); //saisie sécurisée d'un entier
        printf("\n");
        if(cmd>=0 && cmd<=4){ //verification validité commande
            switch (cmd){
                case SET:
                    key=saisieEntier("Saisir une clé");
                    printf("Saisir la valeur: ");
                    scanf("%s", value);
                    break;
                case LOOKUP:
                    key=saisieEntier("Saisire une clé à chercher");
                    strcpy(value,"null"); //on met null car pas besoin de valeur pour un lookup
                    break;
                case DELETE:
                    key=saisieEntier("Saisire une clé à supprimer");
                    strcpy(value,"null"); //pareil
                    break;
                case DUMP:
                    printf("-----------------------------------\n");
                    //ici pas de break exprès, on veut pour un dump affichier la ligne qui précède avant de rendre les variables nulles
                default: // EXIT | DUMP 
                    strcpy(value,"null"); //on met la valeure aa null au cas on arrete a la premiere iteration
                    key=0; //on met la clé a 0 au cas on arrete a la premiere iteration
                    break;
            }

            sendRequest(MASTER, cmd, value, key, IN_PROGRESS); //construction et envoie de la requête
            readRequest(MASTER, &cmd, value, &key, &flag); //réception et deconstruction de la requêtes, 
            // A partir de maintenant key ne vaudra pas la clé, mais le processus qui a repondu au controler
            couleur_reset(); //coloration syntaxique
            printf("\nMessage du Controller (pid %d):\n", getpid());
            // rouge();
            if(cmd!=DUMP && cmd!=EXIT && flag==DONE){ //Message reussite d'une requête, hors dump et exit
                printf("L'opération %d a reussi! (éxecuté par le node n°%d pid:%d)\n", cmd, key, proc_id[key]); //key ici est le node qui a repondu
            }
            else if(cmd==DUMP &&flag==DONE){ //message reussite d'un dump
                printf("L'opération 3 a été reussi par tous les noeuds\n");
            }
            else if(flag==FAIL && cmd!=EXIT){ //message fail d'une requête
                printf("L'opération %d a échouée... (éxecuté par le node n°%d pid:%d)\n", cmd, key, proc_id[key]);
            }
            if(cmd==EXIT && flag==DONE){ //message reussite sauvegarde et fin
                rouge();
                printf("Sauvegarde effectuée.\nBye Bye!\n");
            }
            couleur_reset();//coloration syntaxique
        }
        else{
            printf("Mauvaise commande...\n"); //affichage a caractère informatif de mauvaise commande
        }
    } while (cmd!=EXIT);
        
    fermerDescripteurs(MASTER); //on ferme les descripteurs restants
    
}
