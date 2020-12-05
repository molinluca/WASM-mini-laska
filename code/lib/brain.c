#include "../headers/brain.h"
#include <stdlib.h>
#include "../headers/util/list.h"
static short game_turn = USR_TEAM;
static List *timeline;

int minimax() {
    return 1;
}

void fillStep(Step *s, Piece *p, short dir) {
    if (s != NULL && p != NULL) {
        Piece *hit;
        Cell hc = p->moves[dir].hit;
        s->moved = *p;
        s->last = p->moves[dir];

        hit = getPiece(hc.piece);
        if (hit != NULL) { s->hit = *hit; }
        else             { disposePiece(&(s->hit)); }
    }
}

short play(short i, short dir) {
    Piece* p=getPiece(i);

    if (p==NULL) return 0;
    if (dir<0 || dir>3) return 0;
    if (game_turn==getTeam(p)) {
        calculate(p);

        if (isMoveLegal(&p->moves[dir])) {
            move(p, dir);
            game_turn = (game_turn == CPU_TEAM) ? USR_TEAM : CPU_TEAM;
            return 1;
        }
    }
    return 0;
}

short canPlayerMove(int team) {
    if (team == game_turn) {
        int i, j, pieces=0, moves=0;
        calculateAll(team);

        for (i=0;i<22;i++) {
            Piece* p = getPiece(i);

            if (p!=NULL) {
                if (getTeam(p)==team) {
                    pieces++;
                    for (j=0;j<4;j++) {
                        if (isMoveLegal(&p->moves[j])) moves++;
                    }
                }
            }
        }
        return (short) ( (moves>0) + (pieces>0) );
    }
    return 2;
}

short getCurrentTurn() {
    return game_turn;
}

short bestCPU() {
    if (game_turn == CPU_TEAM) {
        short i, j;
        Piece *p;
        Step step;

        for (i=0; i<22; i++) {
            p = getPiece(i);
            if (p != NULL) {
                for (j=0; j<4; j++) {
                    if (isMoveLegal( &(p->moves[j]) )) {
                        fillStep(&step, p, j);
                        move(p, j);
                        /* minimax */
                        undo(&step);
                    }
                }
            }
        }
    }

    return 0;
}
