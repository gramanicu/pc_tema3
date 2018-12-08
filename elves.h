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
    unsigned char x, y, hp, stamina, dmg;
    char *name;
};

// "creates" a new elf (without giving him any gloves)
void createElf(elf *e, char *name, char x, char y, char hp, char stamina);

// show an elf information - for development purposes
void printElf(elf *e);

#endif