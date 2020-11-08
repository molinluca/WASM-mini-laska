#include "control.c"

void play(short p, short m) {
    Piece *piece;

    /* Checks if the input is "in bounds" */
    if (game_turn != CPU_TEAM && game_turn != USR_TEAM) return;
    if ((p<0 || p>21) || (m<0 || m>3))                  return;
    piece = getPiece(p);

    /* Other checks */
    if (piece == NULL)               return; /* The Piece must not be NULL */
    if (isDisposed(piece))           return; /* The Piece must be alive */
    if (game_turn != getTeam(piece)) return; /* The Piece to move must belong to the team that has to play */

    /* The move must be "legal" */
    calculateAll(game_turn);
    if (piece->moves[m].score < 1) return;

    move(piece, m);
    game_turn = (game_turn == USR_TEAM) ? CPU_TEAM : USR_TEAM;
}
