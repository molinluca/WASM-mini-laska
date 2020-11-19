#include "../headers/control.h"

/* Resets a given Move, setting it to a blank state */
void resetMove(Move *m) {
    if (m == NULL) return;
    m->start.y   = -1;
    m->start.x   = -1;
    m->target.y  = -1;
    m->target.x  = -1;
    m->hit.piece = VOID_CELL;
    m->score     = 0;
}

/* Returns if 2 pieces are in different teams
 * NOTE: the returned value is a boolean-like, either 0 or 1 */
int areEnemies(int a, int b) {
    Piece *pa, *pb;
    if (a < 0 || a > 21 || b < 0 || b > 21) return 0;
    pa = getPiece(a);
    pb = getPiece(b);
    if (pa == NULL || pb == NULL) return 0;
    return getTeam(pa) != getTeam(pb);
}

/* Fills the pointer Move given as parameter only if it leads to a legal Move
 * NOTE: it the move to assert is not legal, it will return leaving it blank */
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
        m->score = 1;
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
        m->score = 2;
        return;
    }

}

/* Calculates all the Moves of a single Piece
 * NOTE: the Piece has not to be NULL or disposed to calculate its moves, otherwise the function won't do anything */
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

/* Calculates all the possible Moves of a team
 * NOTE: The team indicator must be either the CPU_TEAM or the USR_TEAM */
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

/* Tries to perform a move, given the Piece to move and the move index
 * NOTE: if the Piece is disposed or NULL, or if the move is blank, it won't do anything */
void move(Piece *p, int i) {
    Cell *tmp;

    if (i < 0 || i > 4)         return;
    if (p == NULL)              return;
    if (p->moves[i].score <= 0) return;
    if (isVoid(p->y, p->x))     return;
    tmp = cellAt(p->y, p->x);

    fillCell(p->moves[i].target.y, p->moves[i].target.x, *tmp);
    voidCell(p->y, p->x);
    p->y = p->moves[i].target.y;
    p->x = p->moves[i].target.x;

    if (p->tower[0] > 0 && p->y == 6)
        promote(p);
    if (p->tower[0] < 0 && p->y == 0)
        promote(p);

    if (p->moves[i].hit.piece == VOID_CELL) return;
    if (!isDisposed(getPiece(p->moves[i].hit.piece))) {
        conquer(p, getPiece(p->moves[i].hit.piece));
    }
}
