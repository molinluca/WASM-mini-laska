#include <webassembly.h>
#include "headers/game.h"

/** Un "buffer" da 5 interi per rappresentare una pedina [y, x, tower_0, tower_1, tower_2] */
int buffered_piece[5];
/** Un "buffer" da 8 interi per rappresentare le 4 mosse possibili di una pedina
 * [y0, x0, y1, x1, y2, x2, y3, x3]*/
int buffered_moves[8];

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
