#ifndef _LIST_H_
#define _LIST_H_
#include <stdlib.h>

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
typedef struct Node   { void *data; struct Node *next; }Node;
typedef struct List   { Node *head; short len; }List;
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/* Initializes the vector, given its pointer as a parameter
 * NOTE: the List pointer MUST NOT BE NULL!!!*/
int createList(List *v) {
    Node *head = NULL;
    if (v == NULL) {
        return 0;
    }

    head = (Node *) malloc(sizeof(Node));
    if (head == NULL) return 0;

    head->next = NULL;
    v->head = head;
    v->len = 0;
    return 1;
}

/* Deletes the vector (given its pointer) freeing the memory it uses
 * NOTE: this function does NOT free the List itself, but only the memory it contains*/
void freeList(List *v) {
    Node* head;
    Node* temp = NULL;

    if (v == NULL) return;
    head = v->head;

    while (head != NULL) {
        temp = head->next;
        if (head != NULL)
            free(head);
        head = temp;
    }
    v->head = NULL;
    v->len = 0;
}

/* Adds an element at the end of the List
 * NOTE: this function returns 1 if the operation succeeded, otherwise it returns 0*/
int pushList(List *v, void *elem) {
    Node *node = v->head;
    while (node->next != NULL) {
        node = node->next;
    }

    node->next = (Node *) malloc(sizeof(Node));
    if (node->next == NULL) return 0;
    node->next->data = elem;
    node->next->next = NULL;
    v->len += 1;
    return 1;
}

/* Returns the pointer to the data part of the Node
 * NOTE: The returned pointer has to be casted to the preferred value
 * IMPORTANT: this function returns NULL if the List is NULL or if the index overflows the length of the List struct
 */
void *getElementAt(List *v, int index) {
    Node *node = v->head;
    int i      = -1;

    if (index < 0) return NULL;
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
void *popList(List *v, int index) {
    Node *node = v->head;
    Node *prev = NULL;
    void *data = NULL;
    int i      = 0;

    if (index == 0) { return NULL; }

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
    v->len -= 1;
    return data;
}

#endif
