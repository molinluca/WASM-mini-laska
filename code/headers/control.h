#ifndef WASM_MINI_LASKA_CONTROL_H
#define WASM_MINI_LASKA_CONTROL_H
#include "board.h"
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/**
 * Calcola tutte le mosse legali di ogni pedina di uno dei 2 team
 * @param team Il valore del team di cui devono essere calcolate le mosse (CPU_TEAM, USR_TEAM)
 */
void calculateMoves(int team);

/**
 * Clona il contenuto della mossa A nella mossa B
 * @param clone Il puntarore alla mossa clone
 * @param original Il puntatore alla mossa originale
 * @return 1 - Se la clonazione ha avuto effetto
 * @return 0 - Se i puntatori non sono validi e quindi la clonazione non e' andata a buon fine
 */
short cloneMove(Move *clone, Move *original);

/**
 * Clona il contenuto della pedina A nella pedina B
 * @param clone Il puntarore alla pedina clone
 * @param original Il puntatore alla pedina originale
 * @return 1 - Se la clonazione ha avuto effetto
 * @return 0 - Se i puntatori non sono validi e quindi la clonazione non e' andata a buon fine
 */
short clonePiece(Piece *clone, Piece *original);

/**
 * Effettua il movimento di una pedina specifica in una delle 4 direzioni teoricamente possibili
 * @param p Il puntatore alla pedina
 * @param i La direzione [FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT]
 * @note La pedina non viene mossa se il puntatore passato e' NULLO
 * @note Se la direzione porta ad una mossa "non legale", la pedina non viene mossa
 */
void move(Piece *p, int i);

/**
 * Controlla se una mossa in particolare e' legale o meno
 * @param m Il puntatore alla mossa
 * @return 1 - La mossa e' legale, allora puo' essere effettuata in questo momento della partita
 * @return 0 - La mossa e' illegale (o non ha passato i controlli della funzione assertMove(...))
 */
short isMoveLegal(Move *m);

/**
 * Ripristina lo stato precedente del gioco dopo che una mossa e' stata effettuata
 * @param step L'ultima mossa eseguita in formato Step (struct Step)
 * @note Lo step deve essere assolutamente pieno, altrimenti la funzione ritorna immediatamente senza fare nulla
 */
void undo(Step *step);

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#endif
