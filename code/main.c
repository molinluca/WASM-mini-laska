#include <stdio.h>
#include "headers/game.h"

void prt() {
    int i, j;
    for (i=0; i<7; i++) {
        for (j=0; j<7; j++) {
            Cell *c = cellAt(i, j);
            if (c == NULL) continue;

            if (c->piece != VOID_CELL) printf("(%2d) ", c->piece);
            else                       printf("     ");
        }

        printf("\n");
    }

    printf("\n\n");
}

void run_test() {
    /* Run all needed tests of the new features */
}

int main() {
    reset();
    run_test();
    printf("Everything works... press ENTER to quit\n");
    return 0;
}
