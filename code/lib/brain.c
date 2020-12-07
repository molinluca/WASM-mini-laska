#include "../headers/brain.h"
#include <stdlib.h>

int minimax() {
    return 1;
}

short fillStep(Step *s, Piece *p, short dir) {
    if (s != NULL && p != NULL) {
        Piece *hit;
        Cell hc = p->moves[dir].hit;
        if (!clonePiece(&(s->moved), p))              return 0;
        if (!cloneMove(&(s->last), &(p->moves[dir]))) return 0;

        hit = getPiece(hc.piece);
        if (hit != NULL) { return clonePiece(&(s->hit), hit); }
        else             { disposePiece(&(s->hit)); }

        return 1;
    }

    return 0;
}



short canTeamMove(int team, List *l) {
    int i, j;
    int pieces=0, moves=0;

    if (l != NULL) {
        l->head = NULL;
        l->len  = 0;
    }

    calculateAll(team);

    for (i=0; i<22; i++) {
        Piece *p = getPiece(i);
        if (getTeam(p) != team) continue;
        pieces++;

        for (j=0; j<4; j++) {
            Step *step;
            if ( !isMoveLegal(&(p->moves[j])) ) continue;
            moves++;

            if (l == NULL) continue;
            step = malloc(sizeof(Step));

            if (step == NULL) continue;
            fillStep(step, p, (short) j);
            pushList(l, step);
        }
    }

    return (short) ( (moves>0) + (pieces>0) );
}

int eval(Step *step, short depth) {
    return 1;
}
