#ifndef _TYPES_
#define _TYPES_

/* Pieces possible values */
#define CPU          (short)  1
#define USR          (short) -1
#define EMPTY_PIECE  (short)  0
#define PROMOTED_CPU (short)  2
#define PROMOTED_USR (short) -2

/* Cell content */
#define VOID_CELL (short) -1

/* Teams */
#define CPU_TEAM (short) 0
#define USR_TEAM (short) 1
#define DISPOSED (short) 99

/* Game states */
#define STATE_USR_NO_MOVES  (short) 0
#define STATE_CPU_NO_MOVES  (short) 1
#define STATE_USR_NO_PIECES (short) 2
#define STATE_CPU_NO_PIECES (short) 3
#define STATE_GAME_PVP      (short) 10
#define STATE_GAME_PVE_LOW  (short) 11
#define STATE_GAME_PVE_HIGH (short) 12
#define STATE_GAME_NONE     (short) 99

/* Structs types */
typedef struct Coor  { short y, x; }Coor;
typedef struct Cell  { short piece; }Cell;
typedef struct Move  { Coor start, target; Cell hit; short score; }Move;
typedef struct Piece { short y, x; short tower[3]; Move moves[4]; }Piece;

/* Global variables */
static Cell  board[7][7];
static Piece pieces[22];
static short game_state = STATE_GAME_NONE;
static short game_turn  = USR_TEAM;

#endif
