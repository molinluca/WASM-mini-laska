#include "../headers/control.h"

void resetMove(Move *m) {
    if (m == NULL) return;
    m->start.y   = -1;
    m->start.x   = -1;
    m->target.y  = -1;
    m->target.x  = -1;
    m->hit.piece = VOID_CELL;
}

short isMoveLegal(Move *m) {
    if (m != NULL) {
        if (m->start.y < 0  || m->start.y > 6)  return 0;
        if (m->start.x < 0  || m->start.x > 6)  return 0;
        if (m->target.y < 0 || m->target.y > 6) return 0;
        if (m->target.x < 0 || m->target.x > 6) return 0;
        return 1;
    }
    return 0;
}

int areEnemies(int a, int b) {
    Piece *pa, *pb;
    if (a < 0 || a > 21 || b < 0 || b > 21) return 0;
    pa = getPiece(a);
    pb = getPiece(b);
    if (pa == NULL || pb == NULL) return 0;
    return getTeam(pa) != getTeam(pb);
}

void assertMove(Move *m, int y, int x, int modY, int modX) {
    int ny, nx, nny, nnx;
    if (m == NULL) return;
    if (y<0 || y>6 || x<0 || x>6) return;
    resetMove(m);

    ny = y + modY;
    nx = x + modX;
    if (ny<0 || ny>6 || nx<0 || nx>6) return;
    if (isVoid(ny, nx)) {
        m->start.y  = (short) y;
        m->start.x  = (short) x;
        m->target.y = (short) ny;
        m->target.x = (short) nx;
        return;
    }

    if (!areEnemies(cellAt(y, x)->piece, cellAt(ny, nx)->piece)) return;
    nny = ny + modY;
    nnx = nx + modX;
    if (nny<0 || nny>6 || nnx<0 || nnx>6) return;

    if (isVoid(nny, nnx)) {
        m->start.y  = (short) y;
        m->start.x  = (short) x;
        m->target.y = (short) nny;
        m->target.x = (short) nnx;
        m->hit.piece = cellAt(ny, nx)->piece;
        return;
    }

}


short cloneMove(Move *clone, Move *original) {
    if (clone == NULL || original == NULL) return 0;

    clone->start.y   = original->start.y;
    clone->start.x   = original->start.x;
    clone->target.y  = original->target.y;
    clone->target.x  = original->target.x;
    clone->hit.piece = original->hit.piece;

    return 1;
}

short clonePiece(Piece *clone, Piece *original) {
    if (clone == NULL || original == NULL) return 0;

    clone->y        = original->y;
    clone->x        = original->x;
    clone->tower[0] = original->tower[0];
    clone->tower[1] = original->tower[1];
    clone->tower[2] = original->tower[2];
    resetMove(&(clone->moves[0]));
    resetMove(&(clone->moves[1]));
    resetMove(&(clone->moves[2]));

    return 1;
}

void calculate(Piece *p) {
    int mods[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    if (p == NULL) return;
    if (isDisposed(p)) return;

    if (p->tower[0] == USR || p->tower[0] == PROMOTED_USR) {
        /* If is a Player Piece */
        assertMove(&p->moves[0], p->y, p->x, mods[3][0], mods[3][1]); /*FRONT_LEFT*/
        assertMove(&p->moves[1], p->y, p->x, mods[2][0], mods[2][1]); /*FRONT_RIGHT*/

        /* If is promoted */
        if (p->tower[0] == PROMOTED_USR) {
            assertMove(&p->moves[2], p->y, p->x, mods[1][0], mods[1][1]); /*BACK_LEFT*/
            assertMove(&p->moves[3], p->y, p->x, mods[0][0], mods[0][1]); /*BACK_RIGHT*/
        } else {
            resetMove(&p->moves[2]);
            resetMove(&p->moves[3]);
        }
    } else if (p->tower[0] == CPU || p->tower[0] == PROMOTED_CPU) {
        /* If is a Computer Piece */
        assertMove(&p->moves[0], p->y, p->x, mods[1][0], mods[1][1]); /*FRONT_LEFT*/
        assertMove(&p->moves[1], p->y, p->x, mods[0][0], mods[0][1]); /*FRONT_RIGHT*/

        /* If is promoted */
        if (p->tower[0] == PROMOTED_CPU) {
            assertMove(&p->moves[2], p->y, p->x, mods[3][0], mods[3][1]); /*BACK_LEFT*/
            assertMove(&p->moves[3], p->y, p->x, mods[2][0], mods[2][1]); /*BACK_RIGHT*/
        } else {
            resetMove(&p->moves[2]);
            resetMove(&p->moves[3]);
        }
    }
}

void calculateAll(int team) {
    int i;
    Piece *p;
    for (i=0; i<22; i++) {
       p = getPiece(i);
       if (p != NULL) {
          if (getTeam(p) == team) calculate(p);
       }
    }
}

void move(Piece *p, int i) {
    Cell *tmp;

    if (i < 0 || i > 4)         return;
    if (p == NULL)              return;
    if (!isMoveLegal(&p->moves[i])) return;
    if (isVoid(p->y, p->x))     return;
    tmp = cellAt(p->y, p->x);

    fillCell(p->moves[i].target.y, p->moves[i].target.x, *tmp);
    voidCell(p->y, p->x);
    p->y = p->moves[i].target.y;
    p->x = p->moves[i].target.x;

    if (getTeam(p) == CPU_TEAM && p->y == 6) promote(p);
    if (getTeam(p) == USR_TEAM && p->y == 0) promote(p);

    if (p->moves[i].hit.piece == VOID_CELL) return;
    if (!isDisposed(getPiece(p->moves[i].hit.piece))) {
        conquer(p, getPiece(p->moves[i].hit.piece));
    }
}

void undo(Step *step) {
    if (step == NULL) return;
    if (!isDisposed(&(step->moved)) && isMoveLegal(&(step->last))) {
        Cell *c = cellAt(step->last.target.y, step->last.target.x);
        Piece *p;

        if (c != NULL) {
            p = getPiece(c->piece);
            if (p != NULL) {

                clonePiece(p, &(step->moved));
                fillCell(step->last.start.y, step->last.start.x, *c);
                voidCell(step->last.target.y, step->last.target.x);

                if (!isDisposed(&(step->hit))) {
                    Piece *h = getPiece(step->last.hit.piece);
                    if (h != NULL) {

                        clonePiece(h, &(step->hit));
                        fillCell(h->y, h->x, step->last.hit);

                    }
                }
            }
        }
    }
}
