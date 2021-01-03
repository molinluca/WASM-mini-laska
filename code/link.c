#include <webassembly.h>
#include <stdlib.h>
#include "headers/game.h"

/** Un "buffer" da 5 interi per rappresentare una pedina [y, x, tower_0, tower_1, tower_2] */
int buffered_piece[5];
/** Un "buffer" da 8 interi per rappresentare le 4 mosse possibili di una pedina
 * [y0, x0, y1, x1, y2, x2, y3, x3]*/
int buffered_moves[8];
/** Un "buffer" da 11 interi per rappresentare gli "indizi" nel gioco, ovvero le pedine che possono essere mosse */
int buffered_hint[11];

/**
 * Quando viene chiamata, chiama a sua volta la funzione play(...) e ne restituisce il risultato
 * @param p L'indice della pedina da muovere
 * @param i La direzione della mossa
 * @return L'esito della giocata
 */
export short do_move(short p, short i) {
    return playGame(p, i);
}

/**
 * Lancia la funzione per attendere la mossa della CPU per poi restituirne il risultato
 * @return L'esito della giocata della CPU
 */
export short play_CPU() {
    return awaitCPU();
}

/**
 * Riempie correttamente il buffer per le pedine e ne restituisce il puntatore
 * @param i L'indice della pedina
 * @return Il puntatore al buffer
 * @note Se la pedina fosse NULLA o l'indice fuori dal range [0-21]
 * allora il buffer viene riempito da valori pari ad una cella disposta
 */
export int *get_piece(int i) {
    Piece *p;
    buffered_piece[0] = -1;
    buffered_piece[1] = -1;
    buffered_piece[2] = EMPTY_PIECE;
    buffered_piece[3] = EMPTY_PIECE;
    buffered_piece[4] = EMPTY_PIECE;
    if (i<0 || i>21) return buffered_piece;
    p = getPiece(i);
    if (p == NULL) return buffered_piece;

    buffered_piece[0] = p->y;
    buffered_piece[1] = p->x;
    buffered_piece[2] = p->tower[0];
    buffered_piece[3] = p->tower[1];
    buffered_piece[4] = p->tower[2];
    return buffered_piece;
}

/**
 * Riempie il buffer delle mosse per poi restituirne il puntatore
 * @param j l'indice della pedina
 * @return Il puntatore del buffer
 * @note Se l'indice della pedina fosse fuori dal range [0-21] o se la pedina fosse NULL,
 * la funzione riempie il buffer con valori pari a -1
 */
export int *get_moves(int j) {
    int i, team;
    Piece *p;
    for (i=0; i<8; i++) {
        buffered_moves[i] = -1;
    }

    if (j < 0 || j > 21) return buffered_moves;
    if (getCurrentTurn() == CPU_TEAM && getGameState() != STATE_GAME_PVP) return buffered_moves;

    p = getPiece(j);
    team = getTeam(p);

    if (team == getCurrentTurn()) {
        calculateMoves(team);

        if (p == NULL) return buffered_moves;
        for (i = 0; i < 4; i++) {
            if (isMoveLegal(&p->moves[i])) {
                buffered_moves[2 * i]     = p->moves[i].target.y;
                buffered_moves[2 * i + 1] = p->moves[i].target.x;
            }
        }
    }

    return buffered_moves;
}

/**
 * Inizializza una nuova sessione di gioco chiamando la funzione corrispondente.
 * @param type Il valore per il tipo di gioco
 */
export void new_game(int type) {
    initGame((short) type);
}

/**
 * Restituisce il valore che rappresenta lo stato del gioco al momento della chiamata
 * @return Lo stato del gioco
 */
export int get_game_state() {
    return (int) getGameState();
}

/**
 * Termina la sessione di gioco chiamando l'apposita funzione
 */
export void end_game() {
    quitGame();
}

/**
 * Restituisce il valore che rappresenta il turno del gioco al momento della chiamata
 * @return Il turno del gioco
 */
export int get_turn() {
    return getCurrentTurn();
}

/**
 * Restituisce il valore di mobilita' di un giocatore (vedi canTeamMove(...))
 * @return int - Un valore intero compreso nel range [0-2]
 */
export int get_status() {
    return canTeamMove(getCurrentTurn(), NULL);
}

/**
 * Restituisce un buffer da 11 interi contenente tutti gli indici delle pedine che il giocatore puo' muovere
 * rispetto al turno corrente.
 * @return Il buffer degli indici delle pedine giocabili
 * @note Quasi mai le pedine giocabili sono esattamente 11, percio' il buffer viene riempito con tanti elementi
 * quante le pedine giocabili, mentre gli altri elementi vengono impostati a -1
 * @note Gli indici delle pedine giocabili sono condensati all'inizio del buffer, in modo da lasciare una singola
 * coda di -1
 * @note In qualche caso limite nel quale la cella della List ausiliare non viene riempita, semplicente la pedina non
 * viene conteggiata
 */
export int *get_hint() {
    List *l;
    int i, m, team;
    for (i=0; i<11; i++) { buffered_hint[i] = -1; }

    l = createList();
    if (l == NULL) { return buffered_hint; }
    team = getCurrentTurn();
    calculateMoves(team);

    for (i=0; i<22; i++) {
        int flag  = 0;
        int *hint = malloc(sizeof(int));
        Piece *p  = getPiece(i);

        if (hint == NULL)       { continue; }
        if (p == NULL)          { continue; }
        if (getTeam(p) != team) { continue; }

        for (m=0; m<4; m++) {
            if (isMoveLegal( &(p->moves[m]) )) { flag = 1; }
            if (flag) { break; }
        }


        if (flag) {
            *hint = i;
            pushList(l, hint);
        }

        else {
            free(hint);
        }
    }

    for (i=0; i<l->len; i++) { buffered_hint[i] = *( (int *) getElementAt(l, i) ); }

    return buffered_hint;
}
