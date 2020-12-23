#ifndef WASM_MINI_LASKA_ENGINE_H
#define WASM_MINI_LASKA_ENGINE_H
#include "brain.h"
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/**
 * Fa giocare uno dei due team, in particolare muove una pedina in una delle 4 direzioni possibili
 * @param i L'indice della pedina da muovere
 * @param dir La direzione del movimento (FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT)
 * @return 1 - Se e' stato possibile muovere
 * @return 0 - Se nessuna mossa ha avuto luogo
 */
short play(short i, short dir);

/**
 * Restituisce il valore del team che deve giocare il prossimo turno
 * @return CPU_TEAM o USR_TEAM
 */
short getCurrentTurn();

/**
 * Dopo aver calcolato tutte le mosse possibili della CPU, valuta la migliore utilizzando un algoritmo ricorsivo
 * @param depth Profondità alla quale lavora l'algoritmo ricorsivo
 * @return 1 - la CPU ha scelto correttamente la mossa e l'ha eseguita
 * @return 0 - la CPU non è riuscita a muovere
 * @note Se il turno di gioco non corrisponde a quello della CPU la funzione non fa nulla
 */
short playBestCPU(short depth);

/**
 * Deve essere chiamata SOLAMENTE quando si inizializza una nuova sessione di gioco.
 * Inizializza SEMPRE il turno del gicoatore a USR_TEAM
 */
void resetTurn();

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#endif
