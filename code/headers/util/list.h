#ifndef WASM_MINI_LASKA_LIST_H
#define WASM_MINI_LASKA_LIST_H

typedef struct Node { struct Node *next; void *data; } Node;
typedef struct List { Node *head; unsigned len; } List;

/**
 * Crea una nuova lista e la restituisce
 * @return Il puntatore alla lista creata
 * @return NULL se qualcosa non ha funzionato nella creazione della lista
 */
List *createList();

/**
 * Dealloca lo spazio di memoria occupato dalla lista
 * @param l Il puntatore alla lista
 * @note Non fa nulla se il puntatore e' NULLO
 */
void destroyList(List *l);

/**
 * Inserisce un nuovo elemento in coda alla lista
 * @param l Il puntatore alla lista
 * @param data Il puntatore (void *) all'elemento da inserire
 * @return 1 - L'elemento e' stato inserito correttamente
 * @return 0 - Qualcosa non ha funzionato nell'inserimento dell'elemento in coda
 */
int pushList(List *l, void *data);

/**
 * Rimuove l'elemento i-esimo dalla lista e lo restituisce
 * @param l Il puntatore alla lista
 * @param index L'indice dell'elemento
 * @return Il puntatore (void *) dell'elemento rimosso dalla lista
 * @return NULL se qualcosa non ha funzionato (e.g. L'indice non e' valido)
 */
void *popList(List *l, unsigned index);

/**
 * Restituisce l'elemento i-esimo dalla lista senza rimuoverlo
 * @param l Il puntatore alla lista
 * @param index L'indice dell'elemento
 * @return Il puntatore (void *) dell'elemento considerato all'interno della lista
 * @return NULL se qualcosa non ha funzionato (e.g. L'indice non e' valido)
 */
void *getElementAt(List *l, int index);

#endif
