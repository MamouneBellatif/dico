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
#include "../include/dico_pipes.h"
#include "../include/dico_save.h"
#include "../include/dico_node.h"


int checkNode(int key, int node_index){ // 
    /*retourne 0 si le node doit se charger de la clé, 
             retourne un autre entier sinon */
    return (key - node_index) % node_count;
}

int execRequest(int cmd, PTable_entry *table, int key, char* value, int node_index){
   
    /* Execute la commande cmd, sur la Table table, avec eventuellement la clé key
    et la valeur value, Puis retourne en focntion de la reussite ou non de l'execution, un flag
    DONE ou FAIL (representé par un entier (voir macros)*/

    rouge(); //coloration syntaxique
    int flag;
    char *result=lookup(*table,key); // On fait un lookup pour vérifier d'abord si la cle exite ou pas
    switch (cmd){ //si la clé correspond au node
            case SET:
                if (result == NULL){ //si la clé n'existe pas
                    store(table, key, value); //on peut enregistrer la nouvelle entrée
                    printf("Valeur \"%s\" stockée avec la clé %d...\n",value,key);
                    flag=DONE;
                }
                else { //la clé existe donc on retourne fail
                    printf("La clé existe déjà!\n");
                    flag=FAIL;
                }
                
                break;
            case LOOKUP:

                printf("Recherche de valeur (clé \"%d\") -> ",key);
                if (result != NULL){ //si la valeure existe
                    printf("%s\n",result); //on affiche le resultat
                        flag=DONE;
                        couleur_reset();
                }
                else{
                    printf("Non trouvé.\n");
                        flag=FAIL;
                }
                break;

            case DUMP:

                couleur_reset(); //coloration syntaxique 
                syncNode(node_index, IN_PROGRESS, DUMP); //début de la synchronisation
                printf("Affichage de la table du node n°%d:\n",node_index);
                printf("Clé -> valeur \n");
                rouge(); //coloration syntacique
                display(*table);
                printf("-----------------------------------\n");
                syncNode(node_index, DONE, DUMP); //fin de la synchronisation
                flag=DONE;
                break;

            case DELETE:

                if(result!=NULL){ //si ca valeure existe on peut executer la suppression
                    printf("Suppression du couple clé|valeur: %d -> %s\n",key,result);
                    flag=supprimer(table, key);
                }
                else {
                    printf("la clé %d n'existe pas!\n",key);
                    flag=FAIL;
                }
                break;

            default:
                break;
            }
    couleur_reset();
    return flag;
}

void node(int node_index){ //i = index du node (proc fils)
    /* execution principale de chaque processus fils, la fonction est une boucle qui attend de recevoir une requête, 
     , verifie si elle doit l'executer, l'execute, puis repond au controller, 
                            ou fait circuler la reqête (en cas de non execution*/

    char dat_file[]="dico.dat"; //nom du fichier de sauvegarde
    int cmd;//commande a effectuer (voir macros)
    int key; //clé a traiter
    char value[100];//valeure assignée a la clé
    int flag; //statut de la requette, IN_PROGRESS, DONE, FAIL (coir macros)

    PTable_entry table = chargerSauvegarde(dat_file, node_index); //initialisation et affection de la liste chainé a partir de la sauvegarde
    ouvrirDescripteurs(node_index); //fermeture des descripterus intutilisés
    do //début de la boucle d'execution pricnipale du processus fils
    {
        readRequest(node_index, &cmd, value, &key, &flag); //attente et lecture d'une requête puis affectation des variables cmd, key, value et flag
        switch (cmd) //on verifie la cmd pour voir si on continue la boucle
        {
        case EXIT: //utilisateur choisis de quitter
            sauvegarde(dat_file, table, node_index); //on sauvegarde la listes chainés
            fermerDescripteurs(node_index); //on ferme les descrupteurs restants
            break;
        
        default:
            /* On execute la requete SI la requette n'a toujours pas été executé
            ET si la commande est un dump, si ce n'est pas un dump, on verifie bien
            que la clé correspond bien a ce node */
            if(flag==IN_PROGRESS && (cmd==DUMP || checkNode(key, node_index)== 0)){
                    flag=execRequest(cmd, &table, key, value, node_index); 
            }
            if(cmd!=DUMP){
                sendRequest(node_index, cmd, value, key, flag); //on envoie une réponse soit au père, soit au node suivant (la fonction se charge de decider du destinataire)
            }
            break;
        }
    } while (cmd != EXIT);
}