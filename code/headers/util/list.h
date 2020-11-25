#ifndef WASM_MINI_LASKA_LIST_H
#define WASM_MINI_LASKA_LIST_H

typedef struct Node { struct Node *next; void *data; } Node;
typedef struct List { Node *head; unsigned len; } List;

List *createList();
void  destroyList(List *l);

int   pushList(List *l, void *data);
void *getElementAt(List *l, int index);

#endif
