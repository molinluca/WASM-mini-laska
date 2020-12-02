#include "../headers/brain.h"
static short game_turn = USR_TEAM;

short play(short i, short dir){
    Piece* p=getPiece(i);

    if(p==NULL) return 0;
    if(dir<0 || dir>3) return 0;
    if(game_turn==getTeam(p)){
        calculate(p);

        if(isMoveLegal(&p->moves[dir])) {
            move(p, dir);
            game_turn = (game_turn == CPU_TEAM) ? USR_TEAM : CPU_TEAM;
            return 1;
        }
    }
    return 0;
}

short canPlayerMove(int team){
    if(team==game_turn){
        int i, j, pieces=0, moves=0;
        for(i=0;i<22;i++){
            Piece* p=getPiece(i);

            if(p!=NULL){
                if(getTeam(p)==team){
                    pieces++;
                    for(j=0;j<4;j++){
                        if(isMoveLegal(&p->moves[j])) moves++;
                    }
                }
            }
        }
        return (short)((moves>0) + (pieces>0));
    }
    return 2;
}

short getCurrentTurn(){
    return game_turn;
}

