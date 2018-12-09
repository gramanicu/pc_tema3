// Copyright Grama Nicolae 2018
#include <stdlib.h>
#include "game.h"

int main(int argc, char *argv[]) {
    // nu au fost dat numele fisierului de intrare
    if (argc == 1) exit(0);
    startGame(argv[1]);

    return 0;
}
