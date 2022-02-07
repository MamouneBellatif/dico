/* @Author BELLATIF Mamoune & BARKAOUI Meriam */
#ifndef TABLE_H 
#define TABLE_H  
typedef struct _Table_entry { 
   int key;                       // cle 
   // char* val;         //declarer la taille             // valeur 
   char val[100];
   struct _Table_entry *next;     // suivant 
} Table_entry; 
 
typedef Table_entry *PTable_entry;  
char* lookup(PTable_entry table, int k); 
void store(PTable_entry *table, int k, char v[]); 
void display(PTable_entry table);  
#endif