#ifndef _LIB_LASKA_
#define _LIB_LASKA_
#include <webassembly.h>
#include "lib/game.c"

int buffered_cell[3];
int buffered_piece[5];

export
short do_move(short p, short i) {
   Piece *pc;
   Move m;
   if (p > 21 || p < 0) return 0;
   if (i < 0 || i > 3)  return 0;

   calculateAll(CPU_TEAM);
   calculateAll(USR_TEAM);

   pc = getPiece(p);
   if (pc == NULL) return 0;
   m = pc->moves[i];
   if (m.score < 1) return 0;

   doMove(pc, i);
   return 1;
}

export
int *get_piece(int i) {
   Piece *p;
   buffered_piece[0] = -1;
   buffered_piece[1] = -1;
   buffered_piece[2] = EMPTY_PIECE;
   buffered_piece[3] = EMPTY_PIECE;
   buffered_piece[4] = EMPTY_PIECE;
   if (i<0 || i>21) return buffered_piece;
   p = getPiece(i);
   if (p == NULL) return buffered_piece;

   buffered_piece[0] = p->y;
   buffered_piece[1] = p->x;
   buffered_piece[2] = p->tower[0];
   buffered_piece[3] = p->tower[1];
   buffered_piece[4] = p->tower[2];
   return buffered_piece;
}

export
void new_game(int type) {
   initGame((short) type);
}

export
int get_game_state() {
    return (int) game_state;
}

export
void end_game() {
    quitGame();
}

#endif
