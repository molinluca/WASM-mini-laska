#include "../headers/game.h"

static short game_state = STATE_GAME_NONE;
static short algorithm_depth = 0;

void initGame(short type) {
    /* The game must be stopped first in order to init a new one */
    if (game_state != STATE_GAME_NONE) return;

    switch (type) {
        case STATE_GAME_PVE_EASY:
            game_state = type;
            algorithm_depth = DIFF_EASY;
            break;
        case STATE_GAME_PVE_MED:
            game_state = type;
            algorithm_depth = DIFF_MEDIUM;
            break;
        case STATE_GAME_PVE_HARD:
            game_state = type;
            algorithm_depth = DIFF_HARD;
            break;
        case STATE_GAME_PVP:
            game_state = type;
            break;
        default: game_state = STATE_GAME_NONE; return;
    }

    reset();
    resetTurn();
}

/**
 * Aggiorna il flag di stato del gioco tramite gli argomenti passati
 * @param team Il valore del team che deve muovere il prossimo turno
 * @param canMove Un boolean-like per rappresentare se quel team ha possibilita' di muovere o meno
 * @return - Lo stato aggiornato del gioco
 */
short updateState(short team, short canMove){
    if(game_state>=10 && game_state<=13){
        if(team==USR_TEAM){
            if(canMove==0) game_state=STATE_USR_NO_PIECES;
            if(canMove==1) game_state=STATE_USR_NO_MOVES;
        }

        else if(team==CPU_TEAM){
            if(canMove==0) game_state=STATE_CPU_NO_PIECES;
            if(canMove==1) game_state=STATE_CPU_NO_MOVES;
        }
    }
    return game_state;
}

short playGame(short p, short dir){
    short moved, team;

    if(game_state==STATE_GAME_PVP){
        moved=play(p, dir);
        team=getCurrentTurn();
        if (moved) return updateState(team, canTeamMove(team, NULL));
    } else if (game_state>=11 && game_state<=13) {
        team = getCurrentTurn();
        moved = 0;
        if (team == USR_TEAM) moved = play(p, dir);
        if (moved) return updateState(CPU_TEAM, canTeamMove(CPU_TEAM, NULL));
    }
    return STATE_NOT_MOVED;
}

short awaitCPU() {
    if (getCurrentTurn() == CPU_TEAM) {
        int moved = playBestCPU(algorithm_depth);
        if (moved) return updateState(USR_TEAM, canTeamMove(USR_TEAM, NULL));
    }

    return STATE_NOT_MOVED;
}

short getGameState(){
    return game_state;
}

void quitGame() {
    game_state = STATE_GAME_NONE;
}
