#include <stdio.h>
#include "headers/game.h"

/**
 * Funzione usata durante i test, serve semplicemente a stampare gli indici delle varie pedine
 * nelle loro posizioni corrispondenti nella scacchiera
 */
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

/**
 * Funzione creata per eseguire ogni test necessario
 */
void run_test() {
    /* Run all needed tests of the new features */
}

int main() {
    run_test();
    return 0;
}
