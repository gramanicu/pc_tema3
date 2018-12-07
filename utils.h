#ifndef UTILS_H
#define UTILS_H

#define MAX_SIZE 100

typedef struct stormStruct snowstorm;
typedef struct elfStruct elf;
typedef struct mapStruct map;
typedef struct cellStruct cell;

/* cell information
    - glove - the type of glove it has (how much damage it would deal)
    - height - the height of the cell
*/
struct cellStruct {
    unsigned char glove, height;
};

/* map information
    - cell - a 2D array with the "topographic" and loot information
    - radius - the size of the map
*/
struct mapStruct {
    struct cellStruct *cells;
    unsigned char radius;
};

/* snowstorm information
    - x, y - the coordinates of the "storm eye"
    - r - the radius of the storm
    - dmg - how much damage it dealts
*/
struct stormStruct {
    unsigned char x, y, r, dmg;
};

/* elves information
    - x, y : his position
    - hp : health points
    - stamina : movement points
    - dmg : how much damage can he do
          ( what type of gloves does he have )
*/
struct elfStruct {
    unsigned char x, y, hp, stamina, dmg;
    char *name;
};

// parse number k to the snowstorm structure
void parseSnowstorm(int k, snowstorm *s);

// "creates" a new elf (without giving him any gloves)
void createElf(elf *e, char *name, char x, char y, char hp, char stamina);

// return the n'th bit of the number x
unsigned char getBit(int x, int n);

// check if a elf is inside a circle of radius r,
// centered in (0, 0), using the pythagorean theorem
char isInRadius(elf *e, char r);

// some function to alter the map information
void setCellHeight(map *m, int x, int y, char height);
void setCellGloves(map *m, int x, int y, char glove);
void setCell(map *m, int x, int y, char height, char glove);

char getCellHeight(map *m, int x, int y);
char getCellGlove(map *m, int x, int y);

void setMapRadius(map *m, char radius);
void prepareMap(map *m);        // allocates the memory
int mapSize(map *m);
int mapArea(map *m);

void prepareGame(map *m);

// REGION DEVELOPMENT FUNCTIONS

void printSnowstorm(snowstorm *s);
void printElf(elf *e);
void printMapTopographic(map *m);

#endif