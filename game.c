#include "game.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "elves.h"
#include "map.h"

void startGame(char *files) {
    map m;
    elf *players;
    unsigned int playerCount;

    FILE *in, *out;

    prepareFiles(&in, &out, files);
    prepareGame(&m, &players, &playerCount, in, out);
    // Game
    releaseMemory(&m, players, playerCount, in, out);
}

// Reads the input to prepare the game
void prepareGame(map *m, elf **players, unsigned int *count, FILE *in,
                 FILE *out) {
    unsigned int radius, playerCount;

    fscanf(in, "%ud", &radius);
    fscanf(in, "%ud", &playerCount);

    generateMap(m, radius, in);
    *players = calloc(playerCount, sizeof(elf));
    spawnPlayers(*players, playerCount, in);
    checkLanding(*players, &playerCount, m, out);

    *count = playerCount;

    // printPlayers(*players, playerCount);
    // printHeightmap(m);
}

void spawnPlayers(elf *players, unsigned int playerCount, FILE *in) {
    unsigned int i, x, y, hp, stamina;
    char name[50];
    for (i = 0; i < playerCount; i++) {
        fscanf(in, "%s", name);
        fscanf(in, "%ud", &x);
        fscanf(in, "%ud", &y);
        fscanf(in, "%ud", &hp);
        fscanf(in, "%ud", &stamina);
        createElf(players + i, name, x, y, hp, stamina);
    }
}

void printPlayers(elf *players, unsigned int playerCount) {
    unsigned int i;
    for (i = 0; i < playerCount; i++) {
        printElf(players + i);
    }
}

void checkLanding(elf *players, unsigned int *playerCount, map *m, FILE *out) {
    unsigned int i;
    for (i = 0; i < *playerCount; i++) {
        // If he hasn't landed on the glacier
        if (!checkPosition(players + i, m)) {
            fprintf(out, "%s has missed the glacier.\n", (players + i)->name);
            eliminateElf(players, i, playerCount, out);
        }
    }
}

void eliminateElf(elf *players, unsigned int id, unsigned int *count, FILE *out) {
    unsigned int i;
    for(i=id; i<*count-1; i++) {
        elf *aux = (players+i+1);
        releaseElf(players + i);
        createElf(players+i, aux->name, aux->x, aux->y, aux->hp, aux->stamina);
    }
    releaseElf(players + *count - 1);
    *count = *count - 1;

    checkFinished(players ,*count, out);
}

void releaseMemory(map *m, elf *players, unsigned int playerCount, FILE *in,
                   FILE *out) {
    unsigned int i;
    releaseMap(m);
    for (i = 0; i < playerCount; i++) {
        releaseElf(players + i);
    }
    free(players);
    fclose(in);
    fclose(out);
}

void checkFinished(elf *players, unsigned int playerCount, FILE *out) {
    if(playerCount==1) {
        fprintf(out, "%s has won!\n", players->name);
        exit(0);
    }
}