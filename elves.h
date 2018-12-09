#ifndef ELVES_H
#define ELVES_H

typedef struct elfStruct elf;

/* elves information
    - x, y : his position
    - hp : health points
    - stamina : movement points
    - dmg : how much damage can he do
          ( what type of gloves does he have )
*/
struct elfStruct {
    unsigned int x, y, hp, stamina, dmg;
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

// show an elf information - for development purposes
void printElf(elf *e);

#endif