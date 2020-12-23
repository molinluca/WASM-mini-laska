#include "../../headers/util/list.h"
#include <stdlib.h>

List *createList() {
   List *l = malloc(sizeof(List));

   if (l != NULL) {
      l->head = NULL;
      l->len  = 0;
   }

   return l;
}

/**
 * Crea un nuovo Nodo contenente il dato da conservare
 * @param data Il puntatore del dato da conservare
 * @return Il puntatore del NODO (puo' essere NULL se non lo ha creato correttamente)
 */
Node *createNode(void *data) {
   Node *node = malloc(sizeof(Node));
   if (node == NULL) return NULL;

   node->data = data;
   node->next = NULL;
   return node;
}

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
   free(v);
}

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

void *popList(List *l, unsigned index) {
   if (l != NULL) {
      Node *node = l->head;
      Node *prev = NULL;
      void *data = NULL;

      if (l->len < 1) return NULL;
      if (index == 0) {
         data = node->data;
         l->head = node->next;
         l->len -= 1;
         free(node);
         return data;
      } else {
         int i = 0;
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
   }

   return NULL;
}
