#ifndef WASM_MINI_LASKA_GAME_H
#define WASM_MINI_LASKA_GAME_H
#include "engine.h"
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/**
 * Inizializza un nuovo gioco PvP o PvE in base al tipo passato come parametro
 * @param type Il tipo di gioco, STATE_GAME_PVP o STATE_GAME_PVE
 * @note Si puo` gestire solamente un'istanza per volta, se un gioco e` gia inizializzato, questa funzione NON FA NULLA
 */
void initGame(short type);

/**
 * Fa giocare uno dei due team
 * @param p L'indice della pedina da muovere
 * @param dir La direzione in cui muovere la pedina (FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT)
 * @return STATE_NOT_MOVED se la pedina non e` stata mossa (turno sbagliato o movimento non valido)
 * @return Se la mossa ha avuto luogo, ricalcola lo stato del gioco e lo restituisce (sia che esso indichi che la partita e` in corso, sia che indichi che e` terminata)
 */
short playGame(short p, short dir);

short awaitCPU();

/**
 * Restituisce lo stato del gioco al momento della chiamata
 * @return Lo stato corrente del gioco
 */
short getGameState();

/**
 * Termina una partita in corso
 * @note Se non c'e` nessuna partita in corso, non fa NULLA
 */
void quitGame();

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#endif
