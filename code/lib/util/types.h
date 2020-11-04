#ifndef _TYPES_
#define _TYPES_

/* Defined values */
#define CPU          (short)  1
#define USR          (short) -1
#define PROMOTED_CPU (short)  2
#define PROMOTED_USR (short) -2

#define EMPTY_PIECE  (short)  0
#define VOID_CELL    (short) -1
#define CPU_TEAM     (short)  0
#define USR_TEAM     (short)  1

/* Structs variables */
typedef struct Coor  { short y, x; }Coor;
typedef struct Cell  { short piece; }Cell;
typedef struct Move  { Coor start, target; Cell hit; short score; }Move;
typedef struct Piece { short y, x; short tower[3]; Move moves[4]; }Piece;

/* Global variables */
static Cell  board[7][7];
static Piece pieces[22];

#endif
