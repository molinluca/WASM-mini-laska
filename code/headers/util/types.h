#ifndef WASM_MINI_LASKA_H
#define WASM_MINI_LASKA_H
#include <stddef.h>

#define CPU           (1)          /* Il valore per una pedina di CPU */
#define USR          (-1)          /* Il valore per una pedina di USR */
#define EMPTY_PIECE   (0)          /* Il valore per una pedina VUOTA */
#define PROMOTED_CPU  (2)          /* Il valore per una pedina PROMOSSA di CPU */
#define PROMOTED_USR (-2)          /* Il valore per una pedina PROMOSSA di USR */

#define VOID_CELL (-1)             /* Placeholder per una cella che non contiene nessuna pedina */

#define CPU_TEAM (0)               /* Valore per il TEAM di CPU */
#define USR_TEAM (1)               /* Valore per il TEAM di USR */
#define DISPOSED (99)              /* Placeholder per indicare nessun TEAM */

#define FRONT_LEFT  (0)            /* Movimento AVANTI a SINISTRA */
#define FRONT_RIGHT (1)            /* Movimento AVANTI a DESTRA */
#define BACK_LEFT   (2)            /* Movimento INDIETRO a SINISTRA */
#define BACK_RIGHT  (3)            /* Movimento INDIETRO a DESTRA */

#define UNKNOWN_STATE (0)          /* Placeholder per indicare un valore non calcolato dall'algoritmo ricorsivo */
#define INFINITY      (2147483647) /* Il MASSIMO valore per un signed int */

#define STATE_NOT_MOVED     (0)    /* Placeholder per indicare un ritorno in cui nessuna pedina e` stata mossa */
#define STATE_USR_NO_MOVES  (1)    /* Stato di FINE gioco, indica che USR non puo` muovere */
#define STATE_CPU_NO_MOVES  (2)    /* Stato di FINE gioco, indica che CPU non puo` muovere */
#define STATE_USR_NO_PIECES (3)    /* Stato di FINE gioco, indica che USR non ha pedine */
#define STATE_CPU_NO_PIECES (4)    /* Stato di FINE gioco, indica che CPU non ha pedine */
#define STATE_GAME_PVP      (10)   /* Versione BASE del gioco, Player contro Player */
#define STATE_GAME_PVE_EASY (11)   /* Versione Player contro Computer (senza ricorsione) */
#define STATE_GAME_PVE_MED  (12)   /* Versione Player contro Computer con media difficolta` */
#define STATE_GAME_PVE_HARD (13)   /* Versione Player contro Computer molto difficile */
#define STATE_GAME_NONE     (99)   /* Placeholder per indicare NESSUN GIOCO ATTIVO */

#define DIFF_EASY   (0)            /* Per non usare la ricorsione */
#define DIFF_MEDIUM (2)            /* Difficolta` base, 2 mosse nel futuro */
#define DIFF_HARD   (9)            /* Molto difficile, 9 mosse nel futuro */



typedef struct Coor  { short y, x; }Coor;                                 /* Struttura per indicare un set di coordinate (x, y) */
typedef struct Cell  { short piece; }Cell;                                /* Struttura per indicare una CELLA della scacchiera */
typedef struct Move  { Coor start, target; Cell hit; }Move;               /* Struttura per indicare una possibile MOSSA di una PEDINA */
typedef struct Piece { short y, x; short tower[3]; Move moves[4]; }Piece; /* Struttura per indicare una PEDINA del gioco */
typedef struct Step  { Piece moved, hit; Move last; }Step;                /* Struttura per indicare uno STEP (per l'algoritmo ricorsivo) */

#endif
