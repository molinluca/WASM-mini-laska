#ifndef WASM_MINI_LASKA_GAME_H
#define WASM_MINI_LASKA_GAME_H
#include "engine.h"
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/**
 * Inizializza un nuovo gioco PvP o PvE in base al tipo passato come parametro
 * @param type Il tipo di gioco, STATE_GAME_PVP o STATE_GAME_PVE
 * @note Si puo' gestire solamente un'istanza per volta, se un gioco e' gia' inizializzato, questa funzione NON fa nulla
 */
void initGame(short type);

/**
 * Fa giocare uno dei due team
 * @param p L'indice della pedina da muovere
 * @param dir La direzione in cui muovere la pedina (FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT)
 * @return STATE_NOT_MOVED - Se la pedina non e' stata mossa (turno sbagliato o movimento non valido)
 * @return short - Se la mossa e' avvenuta con successo ricalcola lo stato del gioco e lo restituisce
 * (sia che esso indichi che la partita e' ancora in corso, sia che indichi la terminazione)
 */
short playGame(short p, short dir);

/**
 * Cerca di far eseguire la miglior mossa possibile alla CPU basandosi sulla profondita' dell'algoritmo in quella sessione di gioco
 * @return STATE_NOT_MOVED - Se la pedina non e' stata mossa (turno sbagliato o movimento non valido)
 * @return short - Se la mossa e' avvenuta con successo ricalcola lo stato del gioco e lo restituisce
 * (sia che esso indichi che la partita e' ancora in corso, sia che indichi la terminazione)
 */
short awaitCPU();

/**
 * Restituisce lo stato del gioco al momento della chiamata
 * @return short - Lo stato corrente del gioco
 */
short getGameState();

/**
 * Termina una partita in corso
 * @note Se non c'e' nessuna partita in corso, NON fa nulla
 */
void quitGame();

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#endif
