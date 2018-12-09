#include "elves.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "utils.h"

void createElf(elf *e, char *name, int x, int y, int hp, int stamina) {
    e->name = calloc(strlen(name) + 1, sizeof(char));
    strcpy(e->name, name);
    e->x = x;
    e->y = y;
    e->hp = hp;
    e->stamina = stamina;
    e->dmg = 0;
}

void releaseElf(elf *e) { free(e->name); }

int checkPosition(elf *e, map *m) { return isInRadius(e, m->radius); }

void printElf(elf *e) {
    printf("Name : %s\n", e->name);
    printf("Position : (%d, %d)\n", e->x, e->y);
    printf("HP : %d\n", e->hp);
    printf("Stamina : %d\n", e->stamina);
    printf("DMG : %d\n", e->dmg);
}

void setGloves(elf *e, int gloves) { e->dmg = gloves; }

unsigned int getGloves(elf *e) { return e->dmg; }