#include "../headers/engine.h"

static short game_turn = USR_TEAM;

short play(short i, short dir) {
    Piece* p=getPiece(i);

    if (p==NULL) return 0;
    if (dir<0 || dir>3) return 0;
    if (game_turn==getTeam(p)) {
        calculateMoves(getTeam(p));

        if (isMoveLegal( &(p->moves[dir]) )) {
            move(p, dir);
            game_turn = (game_turn == CPU_TEAM) ? USR_TEAM : CPU_TEAM;
            return 1;
        }
    }

    return 0;
}

short playBestCPU(short depth) {
    if (game_turn == CPU_TEAM) {
        int i, best, bestScore, canMove;
        List *l = createList();
        Step *choice;

        if (l == NULL) return 0;
        canMove = canTeamMove(CPU_TEAM, l);

        if (canMove == 2) {
            Step *tmp = getElementAt(l, 0);
            if (tmp != NULL) {
                bestScore = eval(tmp, depth);
                best      = 0;
            }

            for (i=1; i<l->len; i++) {
                int score;
                tmp = getElementAt(l, i);
                if (tmp == NULL) continue;

                score = eval(tmp, depth);
                if (score > bestScore) {
                    bestScore = score;
                    best      = i;
                }
            }

            choice = getElementAt(l, best);
            if (choice == NULL) return 0;


            i = executeStep(choice);
            destroyList(l);

            game_turn = (i) ? USR_TEAM : CPU_TEAM;
            return (short) i;
        }
    }

    return 0;
}

short getCurrentTurn() {
    return game_turn;
}

void resetTurn() {
    game_turn = USR_TEAM;
}
