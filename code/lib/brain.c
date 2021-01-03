#include "../headers/brain.h"
#include <stdlib.h>

/**
 * Restisuisce il MINIMO fra 2 valori INTERI
 * @param a Il primo valore
 * @param b Il secondo valore
 * @return Il minimo fra a e b
 */
int min(int a, int b) {return (a < b) ? a:b;}
/**
 * Restisuisce il MASSIMO fra 2 valori INTERI
 * @param a Il primo valore
 * @param b Il secondo valore
 * @return Il massimo fra a e b
 */
int max(int a, int b) {return (a > b) ? a:b;}

/**
 * Valuta lo stato corrente del gioco mediante una somma complessiva dei valori pesati delle
 * torri di pedine, aggiungendo infine un valore importante per la vittoria eventuale di uno dei 2 team.
 * @return Il valore dello stato corrente del gioco
 */
int evaluateState() {
    int value = 0;
    int i;
    int cpu, usr;

    for (i=0; i<22; i++) {
        Piece *p = getPiece(i);
        if (getTeam(p) == DISPOSED) continue;

        value += p->tower[0] * 9;
        value += p->tower[1] * 5;
        value += p->tower[2] * 1;
    }

    cpu = canTeamMove(CPU_TEAM, NULL);
    usr = canTeamMove(USR_TEAM, NULL);

    if (usr < 2) value += 1000;
    if (cpu < 2) value -= 1000;

    return value;
}

/**
 * Una strategia ricorsiva per stabilire il punteggio massimo di ogni mossa possibile alla CPU, valutando anche le
 * casistiche future.
 * Implementa inoltre una gestione dei "rami" ricorsivi trascurabili tramite la tecnica ALPHA-BETA PRUNING
 * @param depth La profondita' della ricorsione (meglio se minore di 18 per motivi di performance/tempo)
 * @param team Il team da controllare nello step corrente
 * @param alpha Il primo valore per il "pruning" (DEVE essere impostato a -INFINITY alla prima chiamata)
 * @param beta Il secondo valore per il "pruning" (DEVE essere impostato a INFINITY alla prima chiamata)
 * @return Il valore massimo per la mossa
 * @note Questo algoritmo e' stato testato fino a profondita' massima 18 creando delle situazioni
 * di gioco computazionalmente complicate, superata quella soglia il tempo di attesa risulta eccessivo
 * o intollerabile (>10min per mossa)
 * @note Partendo da una profondita' 0 fino ad arrivare ad una soglia massima [9-13] la giocabilita' e' ottimizzata (in rapporto alla difficolta')
 * tenendo conto di tempi di attesa (nel peggiore dei casi) inferiori ai 3 minuti
 */
int minimax(int depth, short team, int alpha, int beta) {
    List *l;
    int end, i, out;

    if (team != CPU_TEAM && team != USR_TEAM) return UNKNOWN_STATE;
    if (depth == 0) return evaluateState();

    l = createList();
    if (l == NULL) return UNKNOWN_STATE;

    end = canTeamMove(team, l);
    if (end < 2) return evaluateState();

    if (team == CPU_TEAM) out = -INFINITY;
    else                  out =  INFINITY;

    for (i=0; i<l->len; i++) {
        Step *s = getElementAt(l, i);
        int tmp;
        if (s == NULL) continue;

        executeStep(s);

        if (team == CPU_TEAM) {
            tmp   = minimax(depth-1, USR_TEAM, alpha, beta);
            out   = max(tmp, out);
            alpha = max(alpha, tmp);
        }

        else {
            tmp  = minimax(depth-1, CPU_TEAM, alpha, beta);
            out  = min(tmp, out);
            beta = min(beta, tmp);
        }

        undo(s);
        if (beta <= alpha) break;
    }

    destroyList(l);
    return out;
}

/**
 * Riempie uno Step con le informazioni ricavate dalla pedina e dalla direzione passati come argomenti
 * @param s Il PUNTATORE allo Step (Step*)
 * @param p Il PUNTATORE alla Pedina (Piece*)
 * @param dir La direzione della mossa
 * @return 1 - Lo Step e' stato riempito correttamente
 * @return 0 - Qualcosa non ha funzionato
 */
short fillStep(Step *s, Piece *p, short dir) {
    if (s != NULL && p != NULL) {
        Piece *hit;
        Cell hc = p->moves[dir].hit;
        if (!clonePiece(&(s->moved), p))              return 0;
        if (!cloneMove(&(s->last), &(p->moves[dir]))) return 0;

        hit = getPiece(hc.piece);
        if (hit != NULL) { return clonePiece(&(s->hit), hit); }
        else             { disposePiece(&(s->hit)); }

        return 1;
    }

    return 0;
}

/**
 * Traduce la direzione di una mossa, dato il suo puntatore ed il team della pedina da muovere
 * @param m Il PUNTATORE alla mossa (Move*)
 * @param team Il valore del team (CPU_TEAM | USR_TEAM)
 * @return int - La direzione della mossa
 * @return -1 - Non e' stato possibile calcolare la direzione della mossa
 */
short translateDirection(Move *m, int team) {
    int diffX, diffY;
    if (m == NULL) return -1;
    diffX = m->target.x - m->start.x;
    diffY = m->target.y - m->start.y;

    if (diffY < 0 && diffX < 0) return (team == CPU_TEAM) ? BACK_LEFT   : FRONT_LEFT;
    if (diffY < 0 && diffX > 0) return (team == CPU_TEAM) ? BACK_RIGHT  : FRONT_RIGHT;
    if (diffY > 0 && diffX < 0) return (team == CPU_TEAM) ? FRONT_LEFT  : BACK_LEFT;
    if (diffY > 0 && diffX > 0) return (team == CPU_TEAM) ? FRONT_RIGHT : BACK_RIGHT;

    return -1;
}

short executeStep(Step *s) {
    if (s != NULL) {
        Piece *moved;
        short direction;

        Cell *c = cellAt(s->moved.y, s->moved.x);
        if (c == NULL) return 0;

        moved = getPiece(c->piece);
        if (getTeam(moved) == DISPOSED) return 0;
        calculateMoves(getTeam(moved));

        direction = translateDirection(&(s->last), getTeam(moved));
        if (direction < 0 || direction > 3) return 0;

        move(moved, direction);
        return 1;
    }

    return 0;
}

short canTeamMove(int team, List *l) {
    int i, j;
    int pieces=0, moves=0;

    if (l != NULL) {
        l->head = NULL;
        l->len  = 0;
    }

    calculateMoves(team);

    for (i=0; i<22; i++) {
        Piece *p = getPiece(i);
        if (getTeam(p) != team) continue;
        pieces++;

        for (j=0; j<4; j++) {
            Step *step;
            if ( !isMoveLegal(&(p->moves[j])) ) continue;
            moves++;

            if (l == NULL) continue;
            step = malloc(sizeof(Step));

            if (step == NULL) continue;
            fillStep(step, p, (short) j);
            pushList(l, step);
        }
    }

    return (short) ( (moves>0) + (pieces>0) );
}

int eval(Step *step, short depth) {
    int value, res;
    if (step == NULL)                return 0;
    if (depth < 0)                   return 0;
    if (isDisposed(&(step->moved)))  return 0;
    if (!isMoveLegal(&(step->last))) return 0;

    res = executeStep(step);
    if (!res) return 0;

    value = minimax(depth, USR_TEAM, -INFINITY, INFINITY);
    undo(step);

    return value;
}
