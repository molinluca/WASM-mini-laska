#include "../headers/engine.h"

static short game_turn = USR_TEAM;

short play(short i, short dir) {
    Piece* p=getPiece(i);

    if (p==NULL) return 0;
    if (dir<0 || dir>3) return 0;
    if (game_turn==getTeam(p)) {
        calculate(p);

        if (isMoveLegal( &(p->moves[dir]) )) {
            move(p, dir);
            game_turn = (game_turn == CPU_TEAM) ? USR_TEAM : CPU_TEAM;
            return 1;
        }
    }

    return 0;
}

short playBestCPU() {
    /* To Be Developed */
    return 0;
}

short getCurrentTurn() {
    return game_turn;
}
