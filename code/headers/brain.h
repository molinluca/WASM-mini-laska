#ifndef WASM_MINI_LASKA_BRAIN_H
#define WASM_MINI_LASKA_BRAIN_H
#include "control.h"
#include "util/list.h"
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/**
 * Controlla se un team in particolare ha mosse disponibili
 * @param team Il team da controllare
 * @param l Il puntatore alla List
 * @return 2 - Se ha mosse legali disponibili
 * @return 1 - Se ha ancora delle pedine, ma non ha mosse legali disponibili
 * @return 0 - Se non ha pedine
 * @note Se il puntatore e' NULL la funzione non lo considera, mentre se punta ad una lista, la riempie con gli step trovati
 */
short canTeamMove(int team, List *l);

/**
 * Valuta una determinata mossa, provando a prevedere nel futuro come si potrebbe evolvere
 * @param step La mossa da valutare (Step)
 * @param depth La profonditaa' della ricerca, ovvero quanti passi nel futuro controllare
 * @return int - Se e' positivo, allora la mossa porta ad una situazione di vantaggio, se e negativo porta ad una situazione di svantaggio
 * @return 0 - Un output particolare: indica che qualcosa e' andato storto durante l'esecuzione della funzione
 */
int eval(Step *step, short depth);

/**
 * Cerca di esguire lo step passato come argomento
 * @param s Step da provare ad eseguire
 * @return 1 - Se riesce ad eseguire lo step
 * @return 0 - Se NON riesce ad eseguire lo step
 * @note Riesce a tradurre dallo step la direzione della mossa della pedina e la pedina stessa.
 * Inoltre supporta la casisista in cui la mossa comprenda una pedina conquistata
 */
short executeStep(Step *s);
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#endif
