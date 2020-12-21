#ifndef WASM_MINI_LASKA_BOARD_H
#define WASM_MINI_LASKA_BOARD_H
#include "util/types.h"
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/**
 * Crea tutte le pedine e le posiziona nella scacchiera.
 * @note In ogni cella e' contenuto un valore numerico, che indica la posizione della pedina corrispondente nell'array di pedine
 * @note Se il valore numerico e' -1 la cella e' ritenuta VUOTA
 */
void reset();

/**
 * Promuove una pedina dato il suo puntatore (se ancora non e` stata promossa)
 * @param p Il puntatore ad una pedina (Piece *)
 * @note Se il puntatore e' nullo, ritorna immediatamente, senza fare niente
 */
void promote(Piece *p);

/**
 * Controlla se la pedina, passato il suo puntatore, e' stata promossa
 * @param p Il puntatore ad una pedina (struct Piece *)
 * @return 1 - Se e' stata promossa
 * @return 0 - Se non lo e' o se il puntatore e' NULLO
 */
int isPromoted(Piece *p);

/**
 * Svuota il contenuto di una cella date le sue coordinate, in pratica pone l'indice della pedina a -1
 * @param y Il valore della riga
 * @param x Il valore della colonna
 * @note Se le coordinate non sono valide (fuori dal range [0, 6]), la funzione ritorna immediatamente senza fare nulla
 */
void voidCell(int y, int x);

/**
 * Copia il contenuto della cella passata come parametro in quella indicata dalle coordinate
 * @param y Il valore della riga
 * @param x Il valore della colonna
 * @param c La cella da "clonare"
 * @note Se le coordinate non sono valide (fuori dal range [0, 6]), la funzione ritorna immediatamente senza fare nulla
 */
void fillCell(int y, int x, Cell c);

/**
 * Controlla se la cella selezionata dalle coordinate e' vuota o meno.
 * @param y Il valore della riga
 * @param x Il valore della colonna
 * @return 1 - Se il contenuto e' -1 o se le coordinate non sono valide (fuori dal range [0, 6])
 * @return 0 - In tutti gli altri casi
 */
int isVoid(int y, int x);

/**
 * Restituisce il puntatore ad una specifica cella, date le coordinate
 * @param y Il valore della riga
 * @param x Il valore della colonna
 * @return Il puntatore alla cella (Cell *)
 * @return NULL se le coordinate non sono valide (fuori dal range [0, 6])
 */
Cell *cellAt(int y, int x);

/**
 * Restituisce il puntatore ad una pedina, dato l'indice della sua posizione nell'array di pedine
 * @param i L'indice della pedina
 * @return Il puntatore alla pedina (Piece *)
 * @return NULL se le coordinate non sono valide (fuori dal range [0, 6])
 */
Piece *getPiece(int i);

/**
 * Restituisce il valore del team che possiede quella determinata pedina, passandone il suo puntatore
 * @param p Il puntatore alla pedina
 * @return CPU_TEAM se appartiene alla squadra della CPU
 * @return USR_TEAM se appartiene alla squadra dell' UTENTE
 * @return DISPOSED se il puntatore e' NULLO o se la pedina e' stata rimossa dalla scacchiera
 */
int getTeam(Piece *p);

/**
 * Rimuove una pedina dalla scacchiera, dato il suo puntatore, e la svuota
 * @param p Il puntatore alla pedina
 * @note Questa funzione non svuota la cella che conteneva la pedina, per farlo si deve usare la funzione voidCell(...)
 */
void disposePiece(Piece *p);

/**
 * Controlla se la pedina non e' piu` "in gioco", dato il suo puntatore
 * @param p Il puntatore alla pedina
 * @return  1 - Se la pedina non contiene nulla, ovvero la testa della torre e' vuota
 * @return  0 - Se la pedina e' ancora in gioco
 * @return -1 - Se il puntatore e' NULL
 */
int isDisposed(Piece *p);

/**
 * Esegue la conquista di una pedina, in pratica prende la testa della torre della pedina "b" e la mette sotto alla torre della pedina "a"
 * @param a Il puntatore alla pedina che conquista
 * @param b Il puntatore alla pedina che viene conquistata
 * @note Se la pedina che deve conquistare ha gia' la torre lunga 3 pezzi, non si incrementa la sua lunghezza ma la si lascia invariata
 * @note Se una delle 2 pedine risulta NON "in gioco", la funzione ritorna immediatamente senza fare nulla
 * @note Se le 2 pedine sono nello stesso team, la funzione ritorna immediatamente senza fare nulla
 */
void conquer(Piece *a, Piece *b);

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#endif
