#include <stdio.h>
#include "elves.h"
#include "game.h"
#include "map.h"

int main() {
    map m;
    elf *players;
    unsigned int playerCount;
    prepareGame(&m, players, &playerCount);
    // Game
    releaseMemory(&m, players, playerCount);

    return 0;
}
