/* @Author BELLATIF Mamoune & BARKAOUI Meriam */
#ifndef MACROS
#define MACROS 

#define MASTER -1 //index du processus père (utilisé en tant qu'argument des fonction pour manipuler les descripteurs)
                 
                    /*Liste des FLAGS (statut de la requette)*/
#define IN_PROGRESS 0 //requete en attente de traitment
#define DONE 1 // requete traité
#define FAIL -1 //requete échouée

                    /* LISTE DES COMMANDES*/
#define EXIT 0
#define SET 1 
#define LOOKUP 2
#define DUMP 3 
#define DELETE 4

#endif // !MACROS
