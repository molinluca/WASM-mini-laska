#include "../headers/board.h"

static Cell  board[7][7];
static Piece pieces[22];

/* First function to run to fully reset the game
 * It generates the 22 Pieces (11 for each team) and placing them in the board */
void reset() {
    short i=0;
    short y, x;
    for (y=0; y<7; y++) {
        for (x = 0; x < 7; x++) {
            if (y < 3 && (x+y)%2 == 0) {
                pieces[i].tower[0] = CPU;
                pieces[i].tower[1] = CPU;
                pieces[i].tower[2] = EMPTY_PIECE;

                pieces[i].y = y;
                pieces[i].x = x;
                board[y][x].piece = i;
                i++;
            } else if (y > 3 && (x+y)%2 == 0) {
                pieces[i].tower[0] = USR;
                pieces[i].tower[1] = USR;
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

/* Promotes the first piece of a tower of pieces */
void promote(Piece *p) {
    if (p == NULL) return;
    if (p->tower[0] == CPU) p->tower[0] = PROMOTED_CPU;
    if (p->tower[0] == USR) p->tower[0] = PROMOTED_USR;
}

/* Given a Piece pointer it returns 1 if the first piece of the tower is promoted, otherwise it returns 0
 * NOTE: returns 0 if the Piece pointer is NULL*/
int isPromoted(Piece *p) {
    if (p == NULL) return 0;
    return !(p->tower[0] % 2);
}

/* Empties a cell of the board, given its coordinates
 * NOTE: It doesn't do anything if the given coordinates are "out of bounds"*/
void voidCell(int y, int x) {
    if (x < 0 || x > 6 || y < 0 || y > 6) return;
    board[y][x].piece = VOID_CELL;
}

/* Fills a Cell, given its coordinates, with the Cell content passed as parameter
 * NOTE: It doesn't do anything if the given coordinates are "out of bounds"*/
void fillCell(int y, int x, Cell c) {
    if (y<0 || y>6 || x<0 || x>6) return;
    /*if (board[y][x].piece != VOID_CELL) return;*/
    board[y][x].piece = c.piece;
}

/* Checks if the pointer to the Piece of the given Cell is NULL. If it is, it returns 1
 * NOTE: if the coordinates are "out of bounds" it returns 1 as well*/
int isVoid(int y, int x) {
    if (x < 0 || x > 6 || y < 0 || y > 6) return 1;
    return (board[y][x].piece == VOID_CELL);
}

/* Returns a Cell given its coordinates
 * NOTE: If the coordinates are "out of bounds" it returns a Void Cell */
Cell *cellAt(int y, int x) {
    if (x < 0 || x > 6 || y < 0 || y > 6) return NULL;
    return &board[y][x];
}

/* Returns a Piece given its index of the Pieces list
 * NOTE: if the index is "out of bounds" it returns a Disposed Piece*/
Piece *getPiece(int i) {
   if (i < 0 || i > 21) return NULL;
   return &pieces[i];
}

/* Returns the team witch the Piece belongs to
 * NOTE: if the piece is NULL or disposed it returns a DISPOSED state*/
int getTeam(Piece *p) {
    if (p == NULL) return DISPOSED;
    if (p->tower[0] == CPU || p->tower[0] == PROMOTED_CPU) return CPU_TEAM;
    if (p->tower[0] == USR || p->tower[0] == PROMOTED_USR) return USR_TEAM;
    return DISPOSED;
}

/* Disposes the selected Piece setting the board coordinates to negative numbers (-1, -1) and the tower to an empty state
 * NOTE: it doesn't do anything if the given Piece pointer is NULL*/
void disposePiece(Piece *p) {
    if (p == NULL) return;
    p->y = -1;
    p->x = -1;
    p->tower[0] = EMPTY_PIECE;
    p->tower[1] = EMPTY_PIECE;
    p->tower[2] = EMPTY_PIECE;
}

/* Checks if a Piece, given its index, have been disposed, and if so it returns 1
 * NOTE: if the given Piece pointer is NULL it returns -1*/
int isDisposed(Piece *p) {
    if (p == NULL) return -1;
    return (p->y == -1 && p->x == -1 && p->tower[0] == EMPTY_PIECE);
}

/* Lets the Piece A to conquer the Piece B stealing the head Piece of B */
void conquer(Piece *a, Piece *b) {
    if (a->tower[0] == EMPTY_PIECE || b->tower[0] == EMPTY_PIECE) return; /* If only one of the 2 Pieces is disposed, then exit */
    if ((a->tower[0]>0) == (b->tower[0]>0)) return;                       /* Checks if the A and B Pieces are in the same team  */

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
