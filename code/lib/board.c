#include "../headers/board.h"

/** La scacchiera, una matrice 7x7 di struct Cell, accessibile unicamente da questo livello del motore di gioco */
static Cell  board[7][7];
/** Le pedine del gioco, gestite in un array di 22 pedine, accessibili unicamente da questo livello del motore di gioco */
static Piece pieces[22];

void reset() {
    short i=0;
    short y, x;
    for (y=0; y<7; y++) {
        for (x = 0; x < 7; x++) {
            if (y < 3 && (x+y)%2 == 0) {
                pieces[i].tower[0] = CPU;
                pieces[i].tower[1] = EMPTY_PIECE;
                pieces[i].tower[2] = EMPTY_PIECE;

                pieces[i].y = y;
                pieces[i].x = x;
                board[y][x].piece = i;
                i++;
            } else if (y > 3 && (x+y)%2 == 0) {
                pieces[i].tower[0] = USR;
                pieces[i].tower[1] = EMPTY_PIECE;
                pieces[i].tower[2] = EMPTY_PIECE;

                pieces[i].y = y;
                pieces[i].x = x;
                board[y][x].piece = i;
                i++;
            } else {
                board[y][x].piece = VOID_CELL;
            }
        }
    }
}

void promote(Piece *p) {
    if (p == NULL) return;
    if (p->tower[0] == CPU) p->tower[0] = PROMOTED_CPU;
    if (p->tower[0] == USR) p->tower[0] = PROMOTED_USR;
}

int isPromoted(Piece *p) {
    if (p == NULL) return 0;
    return !(p->tower[0] % 2);
}

void voidCell(int y, int x) {
    if (x < 0 || x > 6 || y < 0 || y > 6) return;
    board[y][x].piece = VOID_CELL;
}

void fillCell(int y, int x, Cell c) {
    if (y<0 || y>6 || x<0 || x>6) return;
    board[y][x].piece = c.piece;
}

int isVoid(int y, int x) {
    if (x < 0 || x > 6 || y < 0 || y > 6) return 1;
    return (board[y][x].piece == VOID_CELL);
}

Cell *cellAt(int y, int x) {
    if (x < 0 || x > 6 || y < 0 || y > 6) return NULL;
    return &board[y][x];
}

Piece *getPiece(int i) {
   if (i < 0 || i > 21) return NULL;
   return &pieces[i];
}

int getTeam(Piece *p) {
    if (p == NULL) return DISPOSED;
    if (p->tower[0] == CPU || p->tower[0] == PROMOTED_CPU) return CPU_TEAM;
    if (p->tower[0] == USR || p->tower[0] == PROMOTED_USR) return USR_TEAM;
    return DISPOSED;
}

void disposePiece(Piece *p) {
    if (p == NULL) return;
    p->y = -1;
    p->x = -1;
    p->tower[0] = EMPTY_PIECE;
    p->tower[1] = EMPTY_PIECE;
    p->tower[2] = EMPTY_PIECE;
}

int isDisposed(Piece *p) {
    if (p == NULL) return -1;
    return (p->y == -1 && p->x == -1 && p->tower[0] == EMPTY_PIECE);
}

void conquer(Piece *a, Piece *b) {
    if (isDisposed(a) || isDisposed(b)) return;
    if (getTeam(a) == getTeam(b))       return;

    if (a->tower[1] == EMPTY_PIECE) {
        a->tower[1] = b->tower[0];
    } else if (a->tower[2] == EMPTY_PIECE) {
        a->tower[2] = b->tower[0];
    }

    b->tower[0] = b->tower[1];
    b->tower[1] = b->tower[2];
    b->tower[2] = EMPTY_PIECE;
    if (b->tower[0] == EMPTY_PIECE) {
        voidCell(b->y, b->x);
        disposePiece(b);
    }
}
