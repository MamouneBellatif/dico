/* @Author BELLATIF Mamoune & BARKAOUI Meriam */
#ifndef COMMS_H
#define COMMS_H

void readRequest(int i,int *cmd, char *value,int *key, int *flag); //lit la requête et l'interpète et renvoie aussi le buffer original
void sendRequest(int i, int cmd, char *value, int key, int flag);//status flag (0, non traité envoyer au prochai noeud, 1 done: envoyer au père | WAITING | SUCCESS | FAIL
void syncNode(int node_index, int flag, int cmd); //synchronise les nodes pour le dump et la sauvegarde, a apppeler au debut et a la fin de l'execution
#endif // !REQUESTS

