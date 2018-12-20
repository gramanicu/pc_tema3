// Copyright Grama Nicolae 2018
#ifndef ELVES_H
#define ELVES_H
#include "map.h"

typedef struct elfStruct elf;

/* elves information
    - x, y : his position
    - hp : health points
    - stamina : movement points
    - dmg : how much damage can he do
          ( what type of gloves does he have )
    - won : how many fights he has won
    - name : his name
*/
struct elfStruct {
    unsigned int x, y, hp, stamina, dmg, won;
    char *name;
};

// "creates" a new elf (without giving him any gloves)
void createElf(elf *e, char *name, int x, int y, int hp, int stamina);

// Frees the memory used by the elf
void releaseElf(elf *e);

// Gives the elf new gloves
void setGloves(elf *e, int gloves);

// Returns the type of gloves the elf has
unsigned int getGloves(elf *e);

// Sets the elf's stamina
void setStamina(elf *e, int stamina);

// Returns the stamina the elf has
unsigned int getStamina(elf *e);

// Show an elf information - for development purposes
void printElf(elf *e);

// Checks if an elf is still on the glacier
int checkPosition(elf *e, map *m);

// Returns through 2 pointers (x, y) an elf's position
void getPosition(elf *e, unsigned int *x, unsigned int *y);

// Changes the position of an elf
void setPosition(elf *e, unsigned int x, unsigned int y);

// The elf will change gloves (if he needs)
void takeGloves(elf *e, map *m);

// Damage the specified elf for "dmg" damage
void takesDamage(elf *e, unsigned int dmg);

#endif