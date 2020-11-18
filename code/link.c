#ifndef _LIB_LASKA_
#define _LIB_LASKA_
#include <webassembly.h>
#include "lib/game.c"

int buffered_piece[5];
int buffered_moves[8];

export
short do_move(short p, short i) {
   Piece *pc;
   Move m;
   if (p > 21 || p < 0) return 0;
   if (i < 0 || i > 3)  return 0;

   pc = getPiece(p);
   if (pc == NULL) return 0;
   calculate(pc);

   m = pc->moves[i];
   if (m.score < 1) return 0;
   move(pc, i);
   
   return 1;
}

export
void load_moves(int last) {
   /* TO BE CHANGED */
   calculateAll(CPU_TEAM);
   calculateAll(USR_TEAM);
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
int *get_moves(int j) {
   int i;
   Piece *p;
   for (i=0; i<8; i++) {
      buffered_moves[i] = -2;
   }

   if (j < 0 || j > 21) return buffered_moves;

   p = getPiece(j);
   calculate(p);

   if (p == NULL) return buffered_moves;
   for (i=0; i<4; i++) {
      if (!!p->moves[i].score) {
         buffered_moves[2*i]     = p->moves[i].target.y;
         buffered_moves[2*i + 1] = p->moves[i].target.x;
      }
   }
   return buffered_moves;
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
