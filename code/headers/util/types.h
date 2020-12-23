#ifndef WASM_MINI_LASKA_H
#define WASM_MINI_LASKA_H
#include <stddef.h>


/** Il valore per una pedina di CPU */
#define CPU           (1)
/** Il valore per una pedina di USR */
#define USR          (-1)
/** Il valore per una pedina VUOTA */
#define EMPTY_PIECE   (0)
/** Il valore per una pedina PROMOSSA di CPU */
#define PROMOTED_CPU  (2)
/** Il valore per una pedina PROMOSSA di USR */
#define PROMOTED_USR (-2)


/** Placeholder per una cella che non contiene nessuna pedina */
#define VOID_CELL (-1)


/** Valore per il TEAM di CPU */
#define CPU_TEAM (0)
/** Valore per il TEAM di USR */
#define USR_TEAM (1)
/** Placeholder per indicare nessun TEAM */
#define DISPOSED (99)


/** Movimento AVANTI a SINISTRA */
#define FRONT_LEFT  (0)
/** Movimento AVANTI a DESTRA */
#define FRONT_RIGHT (1)
/** Movimento INDIETRO a SINISTRA */
#define BACK_LEFT   (2)
/** Movimento INDIETRO a DESTRA */
#define BACK_RIGHT  (3)


/** Placeholder per indicare un valore non calcolato dall'algoritmo ricorsivo */
#define UNKNOWN_STATE (0)
/** Il MASSIMO valore per un signed int */
#define INFINITY      (2147483647)


/** Placeholder per indicare un ritorno in cui nessuna pedina e` stata mossa */
#define STATE_NOT_MOVED     (0)
/** Stato di FINE gioco, indica che USR non puo' muovere */
#define STATE_USR_NO_MOVES  (1)
/** Stato di FINE gioco, indica che CPU non puo' muovere */
#define STATE_CPU_NO_MOVES  (2)
/** Stato di FINE gioco, indica che USR non ha pedine */
#define STATE_USR_NO_PIECES (3)
/** Stato di FINE gioco, indica che CPU non ha pedine */
#define STATE_CPU_NO_PIECES (4)
/** Versione BASE del gioco, Player contro Player */
#define STATE_GAME_PVP      (10)
/** Versione Player contro Computer (senza ricorsione) */
#define STATE_GAME_PVE_EASY (11)
/** Versione Player contro Computer con media difficolta' */
#define STATE_GAME_PVE_MED  (12)
/** Versione Player contro Computer con difficolta' molto elevata */
#define STATE_GAME_PVE_HARD (13)
/** Placeholder per indicare NESSUN GIOCO ATTIVO */
#define STATE_GAME_NONE     (99)


/** Difficolta' base, per non usare la ricorsione */
#define DIFF_EASY   (0)
/** Difficolta' media, 2 mosse nel futuro */
#define DIFF_MEDIUM (2)
/** Difficolta' molto elevata, 9 mosse nel futuro */
#define DIFF_HARD   (9)




/** Struttura per indicare un set di coordinate (x, y) */
typedef struct Coor  {
/** Il valore per la Y nella coordinata */
    short y;
/** Il valore per la Y nella coordinata */
    short x;
}Coor;


/** Struttura per indicare una CELLA della scacchiera */
typedef struct Cell {
    /** L'indice della pedina */
    short piece;
}Cell;


/** Struttura per indicare una possibile MOSSA di una PEDINA */
typedef struct Move {
    /** La coordinata di partenza */
    Coor start;
    /** La coordinata di arrivo */
    Coor target;
    /** La cella contenente la pedina conquistata */
    Cell hit;
}Move;


/** Struttura per indicare una PEDINA del gioco */
typedef struct Piece {
    /** Posizione Y della pedina */
    short y;
    /** Posizione X della pedina */
    short x;
    /** Il contenitore della torre di pedine */
    short tower[3];
    /** Le 4 mosse possibili (al piu') della pedina */
    Move  moves[4];
}Piece;


/** Struttura per indicare uno STEP (per l'algoritmo ricorsivo) */
typedef struct Step {
    /** La copia della pedina da muovere */
    Piece moved;
    /** La copia della pedina da conquistare */
    Piece hit;
    /** La copia della mossa da eseguire */
    Move  last;
}Step;

#endif
