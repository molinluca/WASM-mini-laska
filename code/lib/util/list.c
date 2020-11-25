#include "../../headers/util/list.h"
#include <stdlib.h>

/* Initializes the vector, given its pointer as a parameter
 * NOTE: the List pointer MUST NOT BE NULL!!!*/
List *createList() {
   List *l = malloc(sizeof(List));

   if (l != NULL) {
      l->head = NULL;
      l->len  = 0;
   }

   return l;
}

Node *createNode(void *data) {
   Node *node = malloc(sizeof(Node));
   if (node == NULL) return NULL;

   node->data = data;
   node->next = NULL;
   return node;
}

/* Deletes the vector (given its pointer) freeing the memory it uses
 * NOTE: this function does NOT free the List itself, but only the memory it contains*/
void destroyList(List *v) {
   Node* head;
   Node* temp = NULL;

   if (v == NULL) return;
   head = v->head;

   while (head != NULL) {
      temp = head->next;
      if (head != NULL) {
         free(head->data);
         free(head);
      }
      head = temp;
   }
   v->head = NULL;
   v->len = 0;
}

/* Adds an element at the end of the List
 * NOTE: this function returns 1 if the operation succeeded, otherwise it returns 0*/
int pushList(List *l, void *data) {
   if (l != NULL) {
      Node *node = l->head;
      if (l->head == NULL) {
         l->head = createNode(data);
         if (l->head == NULL) return 0;

         l->len += 1;
         return 1;
      } else {
         while (node->next != NULL)
            node = node->next;

         node->next = createNode(data);
         if (node->next == NULL) return 0;

         l->len += 1;
         return 1;
      }
   }
   return 0;
}

/* Returns the pointer to the data part of the Node
 * NOTE: The returned pointer has to be casted to the preferred value
 * IMPORTANT: this function returns NULL if the List is NULL or if the index overflows the length of the List struct
 */
void *getElementAt(List *l, int index) {
   Node *node = l->head;
   int i = 0;

   if (index >= l->len || index < 0) return NULL;
   while (node->next != NULL && i < index) {
      node = node->next;
      i++;
   }

   if (i != index) {
      return NULL;
   }

   return node->data;
}

/* Eliminates (pop) an element from the List
 * NOTE: This functions returns the pointer of the data of the deleted Node
 */
void *popList(List *l, unsigned index) {
   Node *node = l->head;
   Node *prev = NULL;
   void *data = NULL;
   int i      = 0;

   /*if (index == 0) { return NULL; }*/

   while (node->next != NULL && i < index) {
      prev = node;
      node = node->next;
      i++;
   }

   if (node == NULL || prev == NULL) { return NULL; }
   if (prev->next != node)           { return NULL; }
   if (i != index)                   { return NULL; }

   prev->next = node->next;
   data = node->data;
   free(node);
   l->len -= 1;
   return data;
}
