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
int checkLanding(elf *players, unsigned int *playerCount, map *m, FILE *out);

// Sets the elf health to 0
void eliminateElf(elf *players, unsigned int id);

/*  Checks if the game is over, says who won and returns 1
    It would have been possible to use exit(0) instead of a return,
    but we need to dealocate the memory before exit.
*/
int checkFinished(elf *players, unsigned int playerCount, FILE *out);

// Check if the elf has fallen off the glacier
char isOut(elf *players, unsigned int id, map *m);

// Check if there is another elf at the specified position (exeption is the id
// of the current elf
int playerAtPosition(elf *players, unsigned int exeption,
                     unsigned int playerCount, unsigned int x, unsigned int y);

// Here is all the game logic
void startGame(char *files);

// Starts the battle between the elves (reads commands and elves moves)
void battle(map *m, elf **players, unsigned int *count, FILE *in, FILE *out);

// Starts a storm at the specified position and damages all players in radius
// Return 1 if the game ended
int startStorm(snowstorm *s, elf *players, unsigned int playerCount, FILE *out);

// Makes the glacier a little smaller, and gives a stamina bonus to all players
// that didn't "got wet because of global warming". Return 1 if game ended
int meltdown(map *m, elf *players, unsigned int staminaBonus,
             unsigned int playerCount, FILE *out);

// Moves the elf to a position, check is he has fallen, takes gloves and attacks
// (if required). Will return 1 if the game is finished
int movePlayer(elf *players, map *m, unsigned int id, unsigned int *playerCount,
               char move, FILE *out);

// Starts the fight between two elves. If the one who got second into the
// cell won, the function returns 1. Else, it returns 0
int fight(elf *att, elf *def);

// Prints the leaderboard, sorted using the asked criteria
void printLeaderboard(elf *players, unsigned int count, FILE *out);

#endif