#include <memory.h>
#include <stdio.h>
#include "elves.h"
#include "game.h"
#include "map.h"

int main() {
    map m;
    elf *players;
    unsigned int playerCount;
    FILE *in = fopen("date.in", "rt");

    prepareGame(&m, &players, &playerCount, in);
    // Game
    releaseMemory(&m, players, playerCount, in);
    return 0;
}
