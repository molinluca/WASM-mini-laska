#include "../headers/brain.h"
static short game_turn = USR_TEAM;

void play(short i, short dir){
    Piece* p=getPiece(i);

    if(p==NULL) return;
    if(dir<0 || dir>3) return;
    if(game_turn==getTeam(p)){
        calculate(p);
        if(p->moves[dir].score>0) {
            move(p, dir);
            game_turn = (game_turn == CPU_TEAM) ? USR_TEAM : CPU_TEAM;
        }
    }
}

