#ifndef WASM_MINI_LASKA_BRAIN_H
#define WASM_MINI_LASKA_BRAIN_H
#include "control.h"
#include "util/list.h"
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/**
 * Controlla se un team in particolare ha mosse disponibili
 * @param team Il team da controllare
 * @param l Il puntatore alla List
 * @return 2 se ha mosse legali disponibili
 * @return 1 se ha ancora delle pedine, ma non ha mosse legali disponibili
 * @return 0 se non ha pedine
 * @note Se il puntatore e` NULL la funzione non lo considera, mentre se punta ad una lista, la riempie con gli Step trovati
 */
short canTeamMove(int team, List *l);

/**
 * Valuta una determinata mossa, provando a prevedere nel futuro come si puo` evolvere
 * @param step La mossa da valutare (Step)
 * @param depth La profondita` della ricerca, ovvero quanti passi nel futuro controllare
 * @return int - Se e` positivo, allora la mossa porta ad una situazione di vantaggio, se e negativo porta ad una situazione di svantaggio
 * @return 0 - Un output particolare, indica che qualcosa e` andato storto durante l'esecuzione della funzione
 */
int eval(Step *step, short depth);

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#endif
