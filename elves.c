#include "elves.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

void createElf(elf *e, char *name, char x, char y, char hp, char stamina) {
    e->name = calloc(strlen(name), sizeof(char));
    strcpy(e->name, name);
    e->x = x;
    e->y = y;
    e->hp = hp;
    e->stamina = stamina;
    e->dmg = 0;
}

void printElf(elf *e) {
    printf("Name : %s\n", e->name);
    printf("Position : (%d, %d)\n", e->x, e->y);
    printf("HP : %d\n", e->hp);
    printf("Stamina : %d\n", e->stamina);
    printf("DMG : %d\n", e->dmg);
}