#include "../headers/game.h"

static short game_state = STATE_GAME_NONE;

void initGame(short type) {
    /* The game must be stopped first in order to init a new one */
    if (game_state != STATE_GAME_NONE) return;

    switch (type) {
        case STATE_GAME_PVE:
        case STATE_GAME_PVP:
            game_state = type;
            break;
        default: game_state = STATE_GAME_NONE; return;
    }

    reset();
}

short updateState(short team, short canMove){
    if(game_state==STATE_GAME_PVP || game_state==STATE_GAME_PVE){
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
    if(game_state==STATE_GAME_PVP){
        short moved, team;
        moved=play(p, dir);
        team=getCurrentTurn();
        if(moved) return updateState(team, canPlayerMove(team));
    }
    return STATE_NOT_MOVED;
}

short getGameState(){
    return game_state;
}

void quitGame() {
    game_state = STATE_GAME_NONE;
}
