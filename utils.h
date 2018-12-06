#ifndef UTILS_H
#define UTILS_H

typedef struct stormStruct snowstorm;
typedef struct elfStruct elf;

// snowstorms information
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
};

// parse number k to the snowstorm structure
void parseSnowstorm(int k, snowstorm *s);

// "creates" a new elf (without giving him any gloves)
void createElf(elf *e, int x, int y, int hp, int stamina);

// return the n'th bit of the number x
unsigned char getBit(int x, int n);

// check if a elf is inside a circle of radius r,
// centered in (0, 0), using the pythagorean theorem
char isInRadius(elf *e, int r);

// REGION DEVELOPMENT

// print snowstorm info
void printSnowstorm(snowstorm *s);
void printElf(elf *e);

#endif