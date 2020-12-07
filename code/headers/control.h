#ifndef WASM_MINI_LASKA_CONTROL_H
#define WASM_MINI_LASKA_CONTROL_H
#include "board.h"
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/**
 * Calcola tutte le mosse legali che una pedina puo' fare
 * @param p Il puntatore alla pedina
 */
void calculate(Piece *p);

/**
 * Calcola tutte le mosse legali di ogni pedina di uno dei 2 team
 * @param team Il valore del team di cui calcolare le mosse (CPU_TEAM, USR_TEAM)
 */
void calculateAll(int team);

/**
 * Clona il contenuto della mossa A alla mossa B
 * @param clone Il puntarore alla mossa clone
 * @param original Il puntatore alla mossa originale
 * @return 1 se la clonazione ha avuto effetto
 * @return 0 se i puntatori non sono validi
 */
short cloneMove(Move *clone, Move *original);

/**
 * Clona il contenuto della pedina A alla pedina B
 * @param clone Il puntarore alla pedina clone
 * @param original Il puntatore alla pedina originale
 * @return 1 se la clonazione ha avuto effetto
 * @return 0 se i puntatori non sono validi
 */
short clonePiece(Piece *clone, Piece *original);

/**
 * Effettua il movimento di una pedina specifica in una delle 4 direzioni
 * @param p Il puntatore alla pedina
 * @param i La direzione [FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT]
 * @note La pedina non viene mossa se il puntatore passato e` NULLO
 * @note Se la direzione porta ad una mossa "non legale", non muove nulla
 */
void move(Piece *p, int i);

/**
 * Controlla se una mossa in particolare e` legale o meno
 * @param m Il puntatore alla mossa
 * @return 1 la mossa e` legale, puo' essere effettuata in questo momento dela partita
 * @return 0 la mossa e` illegale (o non ha passato i controlli della funzione assertMove(...))
 */
short isMoveLegal(Move *m);

void undo(Step *step);

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#endif
