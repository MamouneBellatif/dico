/* @Author BELLATIF Mamoune & BARKAOUI Meriam */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "../include/dico_pipes.h"
#include "../include/dico_controller.h"
#include "../include/dico_vars.h"
#include "../include/dico_node.h"

int node_count; // nombre de node N 
int (*f_node)[2];//tableau de descripteur (un descripteur par node )
int f_master[2]; //descripteurs controller


int main(int argc, char **argv){
    if (argc==2 && atoi(argv[1])>=2){ //on s'assure que n est bien un entier superieur ou egal a 2
        init(atoi(argv[1])); //initialisation des variables globales
        int *proc_id=malloc(node_count*sizeof(*proc_id)); ////tableau de pid des processus fils
        int pid; //pour le retour de fork
        for (int i = 0; i < node_count; i++){ //création des nodes
            switch (pid=fork()) 
            {
                case -1: //cas erreur
                    perror("erreur fork");
                    exit(-1);
                case 0: //cas fils
                    node(i);
                    exit(0);    
                default: 
                proc_id[i]=pid; //on garde en mémoire les pid des processus fils (utilisé par controller pour connaitre le pid des nodes)
                    break;
            }
        }
        controller(proc_id); 
        while (wait(NULL)!=-1); 
        return 0;
    }
    else {
        printf("Usage: %s <(N>1)>\n",argv[0]); //cas mauvais argument
        exit(-1);
    }
}