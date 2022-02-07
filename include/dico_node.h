/* @Author BELLATIF Mamoune & BARKAOUI Meriam */
#include "table.h"
#ifndef NODE_H
#define NODE_H


int checkNode(int key, int node_index);
int execRequest(int cmd, PTable_entry *table, int key, char* value, int node_index);
void node(int node_index);

#endif // !NODE_H