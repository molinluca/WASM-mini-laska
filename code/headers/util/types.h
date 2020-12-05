#ifndef WASM_MINI_LASKA_H
#define WASM_MINI_LASKA_H
#include <stddef.h>

/* Pieces possible values */
#define CPU           (1)
#define USR          (-1)
#define EMPTY_PIECE   (0)
#define PROMOTED_CPU  (2)
#define PROMOTED_USR (-2)

/* Cell content */
#define VOID_CELL (-1)

/* Teams */
#define CPU_TEAM (0)
#define USR_TEAM (1)
#define DISPOSED (99)

/* Movement */
#define FRONT_LEFT  (0)
#define FRONT_RIGHT (1)
#define BACK_LEFT   (2)
#define BACK_RIGHT  (3)

/* Game states */
#define STATE_NOT_MOVED     (0)
#define STATE_USR_NO_MOVES  (1)
#define STATE_CPU_NO_MOVES  (2)
#define STATE_USR_NO_PIECES (3)
#define STATE_CPU_NO_PIECES (4)
#define STATE_GAME_PVP      (10)
#define STATE_GAME_PVE      (11)
#define STATE_GAME_NONE     (99)

/* Structs types */
typedef struct Coor  { short y, x; }Coor;
typedef struct Cell  { short piece; }Cell;
typedef struct Move  { Coor start, target; Cell hit; }Move;
typedef struct Piece { short y, x; short tower[3]; Move moves[4]; }Piece;
typedef struct Score { Piece *piece; short direction; int score; }Score;

#endif
