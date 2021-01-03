#include "../headers/control.h"

/** Un "flag" per determinare la possibilita' di forzare la conquista di una pedina nel calcolo delle mosse */
static int force_eat = -1;

/**
 * Svuota completamente il contenuto di una mossa passata come argomento (le coordinate [-1, -1] indicano una
 * posizione sicuramente FUORI dalla scacchiera)
 * @param m Il PUNTATORE alla mossa (Move*)
 */
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

/**
 * Determina se 2 pedine in particolare appartengono a team diversi fra loro.
 * @param a L'indice della pedina A
 * @param b L'indice della pedina B
 * @return 1 - Le pedine sono di 2 team diversi
 * @return 0 - Le pedine appartengono allo stesso team
 */
int areEnemies(int a, int b) {
    Piece *pa, *pb;
    if (a < 0 || a > 21 || b < 0 || b > 21) return 0;
    pa = getPiece(a);
    pb = getPiece(b);
    if (pa == NULL || pb == NULL) return 0;
    return getTeam(pa) != getTeam(pb);
}

/**
 * Prova a validare una mossa in base agli argomenti passati, se gli argomenti dovessero creare una mossa non valida,
 * quest'ultima viene svuotata completamente
 * @param m Il PUNTATORE alla mossa (Move*)
 * @param y La posizione iniziale della pedina (Y)
 * @param x La posizione iniziale della pedina (X)
 * @param modY Il valore di spostamento in Y della pedina (-1 | 1)
 * @param modX Il valore di spostamento in X della pedina (-1 | 1)
 */
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
    resetMove(&(clone->moves[3]));

    return 1;
}

/**
 * Tenta di calcolare una mossa di una specifica pedina in una specifica direzione
 * @param p Il PUNTATORE alla pedina (Piece*)
 * @param dir La direzione della mossa
 */
void calc(Piece *p, int dir) {
    int delta[4][2] = {{1, -1}, {1, 1}, {-1, -1}, {-1, 1}};
    int d;
    int team = getTeam(p);

    if (team == DISPOSED) return;
    if (dir < 0 || dir > 3) return;

    if (team == CPU_TEAM) d = dir;
    else                  d = (dir + 2) % 4;

    assertMove(&(p->moves[dir]), p->y, p->x, delta[d][0], delta[d][1]);

    if (force_eat) {
        if (p->moves[dir].hit.piece == VOID_CELL) resetMove(&(p->moves[dir]));
        else force_eat = 1;
    }
}

void calculateMoves(int team) {
    force_eat = -1;

    loop_all: { /* Un etichetta per evitare di riscrivere lo stesso codice 2 volte, intercettata dalla seconda volta da "goto loop_all;" */
        int i, j;
        for (i=0; i<22; i++) {
            Piece *p = getPiece(i);
            if (getTeam(p) != team) continue;

            for (j=0; j<4; j++) {
                resetMove(&(p->moves[j]));
                if (!isPromoted(p) && j > 1) continue;
                calc(p, j);
            }
        }
    };

    if (force_eat < 0) {
        force_eat = 0;
        goto loop_all; /* Lo stesso principio dell'istruzione "j" in architettura degli elaboratori (salto incondizionato)  */
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
    force_eat = -1;

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
