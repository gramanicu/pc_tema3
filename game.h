#ifndef GAME_H
#define GAME_H
#include "elves.h"
#include "map.h"
#include "utils.h"

/*  It will prepare the game and "return" (using pointers)
    the map, players and player count, so we can deallocate the
    memory at the end of execution
*/
void prepareGame(map *m, elf **players, unsigned int *count, FILE *in);

// Frees all dinamically allocated memory and closes the input file
void releaseMemory(map *m, elf *players, unsigned int playerCount, FILE *in,
                   FILE *out);

// Reads player information from the file and spawns the elves
void spawnPlayers(elf *players, unsigned int playerCount, FILE *in);

// Prints player information - development purposes
void printPlayers(elf *players, unsigned int playerCount);

// Check if any player has missed the glacier
void checkLanding(elf *players, unsigned int playerCount, map *m);

// Here is all the game logic
void startGame(char *files);

#endif