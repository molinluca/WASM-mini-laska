#include <stdlib.h>
#include "util/types.h"
#include "brain.c"


void run_test(int j) {
    int i;
    calculateAll(CPU_TEAM);
    calculateAll(USR_TEAM);
    for (i=0; i<4; i++) {
        if (pieces[j].moves[i].score > 0) {
            doMove(&pieces[j], i);
            break;
        }
    }
}
