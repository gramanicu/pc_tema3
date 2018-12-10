#ifndef GAME_H
#define GAME_H
#include "elves.h"
#include "map.h"
#include "utils.h"

/*  It will prepare the game and "return" (using pointers)
    the map, players and player count, so we can deallocate the
    memory at the end of execution
*/
void prepareGame(map *m, elf **players, unsigned int *count, FILE *in,
                 FILE *out);

// Frees all dinamically allocated memory and closes the input file
void releaseMemory(map *m, elf *players, unsigned int playerCount, FILE *in,
                   FILE *out);

// Reads player information from the file and spawns the elves
void spawnPlayers(elf *players, unsigned int playerCount, FILE *in);

// Prints player information - development purposes
void printPlayers(elf *players, unsigned int playerCount);

// Check if any player has missed the glacier
void checkLanding(elf *players, unsigned int *playerCount, map *m, FILE *out);

// Sends an elf back to santa and modifies the total number of players
void eliminateElf(elf *players, unsigned int id, unsigned int *count,
                  FILE *out);

// Checks if the game is over, says who won and closes the program
void checkFinished(elf *players, unsigned int playerCount, FILE *out);

// Check if the elf has fallen off the glacier
char isOut(elf *players, unsigned int id, map *m);

// Check if there is another elf at the specified position (exeption is the id
// of the current elf
int playerAtPosition(elf *players, unsigned int exeption,
                     unsigned int playerCount, unsigned int x, unsigned int y);

// Here is all the game logic
void startGame(char *files);

// Starts the fight between two elves. If the one who got second into the cell
// won, the function returns 1. Else, it returns 0
int fight(elf *att, elf *def)

#endif