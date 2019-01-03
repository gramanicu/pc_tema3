// Copyright Grama Nicolae 2018
#ifndef UTILS_H
#define UTILS_H
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100
#define uint unsigned int

typedef struct elfStruct elf;
typedef struct mapStruct map;
typedef struct cellStruct cell;
typedef struct stormStruct snowstorm;

// Return the n'th bit of the number x
unsigned char getBit(int x, int n);

// Check if a elf is inside a circle of radius r,
// centered in (c, c), using the pythagorean theorem
char isInRadius(elf *e, uint c, uint r);

void prepareFiles(FILE **in, FILE **out);

// -------------- Elves Region -----------------

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
    uint x, y, hp, stamina, dmg, won;
    char *name;
};

// "creates" a new elf (without giving him any gloves)
void createElf(elf *e, char *name, int x, int y, int hp, int stamina);

// Frees the memory used by the elf
void releaseElf(elf *e);

// Gives the elf new gloves
void setGloves(elf *e, int gloves);

// Returns the type of gloves the elf has
uint getGloves(elf *e);

// Sets the elf's stamina
void setStamina(elf *e, int stamina);

// Returns the stamina the elf has
uint getStamina(elf *e);

// Show an elf information - for development purposes
void printElf(elf *e);

// Checks if an elf is still on the glacier
int checkPosition(elf *e, map *m);

// Returns through 2 pointers (x, y) an elf's position
void getPosition(elf *e, uint *x, uint *y);

// Changes the position of an elf
void setPosition(elf *e, uint x, uint y);

// The elf will change gloves (if he needs)
void takeGloves(elf *e, map *m);

// Damage the specified elf for "dmg" damage
void takesDamage(elf *e, uint dmg);

// -------------------------------------------

// -------------- Map Region -----------------

/* snowstorm information
    - x, y - the coordinates of the "storm eye"
    - r - the radius of the storm
    - dmg - how much damage it dealts
*/
struct stormStruct {
    unsigned char x, y, r, dmg;
};

/* cell information
    - glove - the type of glove it has (how much damage it would deal)
    - height - the height of the cell
*/
struct cellStruct {
    uint glove, height;
};

/* map information
    - cell - a 2D array with the "topographic" and loot information
    - radius - radius of the map
    - diameter - how long each side of the map is
    - center - the map center (the x and y are equal)
*/
struct mapStruct {
    struct cellStruct *cells;
    uint diameter;
    uint radius;
    uint center;
};

// Access a specific cell from the map (making adjustements
// for in case the coordinates are relative to the original map)
cell *accessCell(map *m, int x, int y);

// Counts how many meltdown the glacier has suffered
uint meltdownsCounter(map *m);

// Parse number k to the snowstorm structure
void parseSnowstorm(int k, snowstorm *s);

// Shows a snowstorm info - for development purposes
void printSnowstorm(snowstorm *s);

// Set a map cell height
void setCellHeight(map *m, int x, int y, uint height);

// Set the glove type in a cell
void setCellGloves(map *m, int x, int y, uint glove);

// Set both the cell height and glove type
void setCell(map *m, int x, int y, uint height, unsigned intglove);

// Returns a cell height or -1 (if the location is out)
int getCellHeight(map *m, int x, int y);

// Returns a cell glove type
uint getCellGloves(map *m, int x, int y);

// Returns the map diameter (size of one side of the square)
uint mapDiameter(map *m);

// Returns the map area
uint mapArea(map *m);

// Returns the original diameter
uint mapOriginalDiameter(map *m);

// Allocates the memory for the map and sets the radius
void prepareMap(map *m, uint radius, uint center);

// Deallocates the memory used by the map
void releaseMap(map *m);

// Melts the glacier
void reduceMap(map *origin);

// Generates the map, with a specified radius and center
// taking required data from a FILE
void generateMap(map *m, uint radius, uint center, FILE *in);

// Shows a map's heightmap - for development purposes
void printHeightmap(map *m);

// Show all the gloves on the map - for development purposes
void printGlovemap(map *m);

// -------------------------------------------

#endif