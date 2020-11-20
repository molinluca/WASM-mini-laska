#ifndef WASM_MINI_LASKA_BOARD_H
#define WASM_MINI_LASKA_BOARD_H
#include "types.h"
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

void   reset();
void   promote(Piece *p);
int    isPromoted(Piece *p);
void   voidCell(int y, int x);
void   fillCell(int y, int x, Cell c);
int    isVoid(int y, int x);
Cell  *cellAt(int y, int x);
Piece *getPiece(int i);
int    getTeam(Piece *p);
void   disposePiece(Piece *p);
int    isDisposed(Piece *p);
void   conquer(Piece *a, Piece *b);

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#endif
