#ifndef WASM_MINI_LASKA_ENGINE_H
#define WASM_MINI_LASKA_ENGINE_H
#include "brain.h"
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
 * Restituisce il valore del team che deve giocare il prossimo turno
 * @return CPU_TEAM o USR_TEAM
 */
short getCurrentTurn();

short playBestCPU(short depth);

void resetTurn();

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#endif
