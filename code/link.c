#ifndef _LIB_LASKA_
#define _LIB_LASKA_
#include <webassembly.h>
#include "headers/game.h"

int buffered_piece[5];
int buffered_moves[8];

export
short do_move(short p, short i) {
    return playGame(p, i);
}

export
short play_CPU() {
    return awaitCPU();
}

export
int *get_piece(int i) {
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

export
int *get_moves(int j) {
    int i;
    Piece *p;
    for (i=0; i<8; i++) {
        buffered_moves[i] = -1;
    }

    if (j < 0 || j > 21) return buffered_moves;
    if (getCurrentTurn() == CPU_TEAM && getGameState() != STATE_GAME_PVP) return buffered_moves;

    p = getPiece(j);
    if (getTeam(p) == getCurrentTurn()) {
        calculate(p);

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

export
void new_game(int type) {
    initGame((short) type);
}

export
int get_game_state() {
    return (int) getGameState();
}

export
void end_game() {
    quitGame();
}

export
int get_turn() {
    return getCurrentTurn();
}

export
int get_status() {
    return canTeamMove(getCurrentTurn(), ((void *)0));
}

#endif
