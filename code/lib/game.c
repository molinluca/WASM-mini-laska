#ifndef _GAME_
#define _GAME_
#include <stdlib.h>
#include "util/types.h"
#include "brain.c"

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

void initGame(short type) {
    /* The game must be stopped first in order to init a new one */
    if (game_state != STATE_GAME_NONE) return;

    switch (type) {
        case STATE_GAME_PVE_HIGH:
        case STATE_GAME_PVE_LOW:
        case STATE_GAME_PVP:
            game_state = type;
            break;
        default: game_state = STATE_GAME_NONE; return;
    }

    reset();
    calculateAll(CPU_TEAM);
    calculateAll(USR_TEAM);
}

void changeState(short type) {
    /* The game must be running in order to change its state */
    if (game_state == STATE_GAME_NONE) return;

    switch (type) {
        case STATE_CPU_NO_MOVES:
        case STATE_CPU_NO_PIECES:
        case STATE_USR_NO_MOVES:
        case STATE_USR_NO_PIECES:
            game_state = type;
            break;
        default: return;
    }
}

void quitGame() {
    game_state = STATE_GAME_NONE;
}

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#endif
