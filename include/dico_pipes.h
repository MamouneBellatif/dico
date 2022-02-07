/* @Author BELLATIF Mamoune & BARKAOUI Meriam */
#ifndef PIPES_H
#define PIPES_H

void init(int node_count);
void fermerDescripteurs(int i);//@ferme les descripteurs utilié par le node i
void ouvrirDescripteurs(int i); //ferme touts les descripteurs sauf ceux qu'on souhaite utiliser

#endif // !PIPES