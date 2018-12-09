#ifndef MAP_H
#define MAP_H
#include <stdio.h>

typedef struct mapStruct map;
typedef struct cellStruct cell;
typedef struct stormStruct snowstorm;

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
    unsigned int glove, height;
};

/* map information
    - cell - a 2D array with the "topographic" and loot information
    - radius - radius of the map
    - diameter - how long each side of the map is
*/
struct mapStruct {
    struct cellStruct *cells;
    unsigned int diameter;
    unsigned int radius;
};

// Parse number k to the snowstorm structure
void parseSnowstorm(int k, snowstorm *s);

// Shows a snowstorm info - for development purposes
void printSnowstorm(snowstorm *s);

// Set a map cell height
void setCellHeight(map *m, int x, int y, unsigned int height);

// Set the glove type in a cell
void setCellGloves(map *m, int x, int y, char glove);

// Set both the cell height and glove type
void setCell(map *m, int x, int y, unsigned int height, char glove);

// Returns a cell height
unsigned int getCellHeight(map *m, int x, int y);

// Returns a cell glove type
unsigned int getCellGlove(map *m, int x, int y);

// Returns the map diameter (size of one side of the square)
unsigned int mapDiameter(map *m);

// Returns the map area
unsigned int mapArea(map *m);

// Allocates the memory for the map and sets the radius
void prepareMap(map *m, unsigned int radius);

// Deallocates the memory used by the map
void releaseMap(map *m);

// Generates the map, with a specified radius
// taking required data from a FILE
void generateMap(map *m, unsigned int radius, FILE *in);

// Shows a map's heightmap - for development purposes
void printHeightmap(map *m);

#endif