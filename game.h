#ifndef GAME_H
#define GAME_H
#include "elves.h"
#include "map.h"
#include "utils.h"

/*  It will prepare the game and "return" (using pointers)
    the map, players and player count, so we can deallocate the
    memory at the end of execution
*/
void prepareGame(map *m, elf *players, unsigned int *count);

// Frees all dinamically allocated memory
void releaseMemory(map *m, elf *players, unsigned int playerCount);

// Reads player information from the file and spawns the elves
void spawnPlayers(elf *players, unsigned int playerCount, FILE *in);

// Prints player information - development purposes
void printPlayers(elf *players, unsigned int playerCount);

#endif