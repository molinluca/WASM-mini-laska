#ifndef WASM_MINI_LASKA_BRAIN_H
#define WASM_MINI_LASKA_BRAIN_H
#include "control.h"
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/**
 * Fa giocare uno dei due team, in particolare muove una pedina in una delle 4 possibili direzioni
 * @param i l'indice della pedina da muovere
 * @param dir la direzione del movimento (FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT)
 * @return 1 se e` stato possibile muovere
 * @return 0 altrimenti
 */
short play(short i, short dir);

/**
 * Controlla se un team in particolare ha mosse disponibili
 * @param team Il team da controllare
 * @return 2 se ha mosse legali disponibili
 * @return 1 se ha ancora delle pedine, ma non ha mosse legali disponibili
 * @return 0 se non ha pedine
 */
short canPlayerMove(int team);

/**
 * Restituisce il valore del team che deve giocare il prossimo turno
 * @return CPU_TEAM o USR_TEAM
 */
short getCurrentTurn();

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#endif
