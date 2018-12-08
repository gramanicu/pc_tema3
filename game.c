#include "game.h"
#include "map.h"
#include "elves.h"
#include "utils.h"
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

void prepareGame(map *m) {
    FILE *in = fopen("date.in", "rt");
    unsigned int radius;
    char playerCount;
    elf *players;

    fscanf(in, "%ud", &radius);

    fscanf(in, "%c", &playerCount);
    players = calloc(playerCount, sizeof(elf));

    generateMap(m, radius, in);
    printHeightmap(m);

}