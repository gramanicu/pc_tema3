// Copyright Grama Nicolae 2018
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
    e->won = 0;
}

void releaseElf(elf *e) { free(e->name); }

int checkPosition(elf *e, map *m) {
    return isInRadius(e, m->center, m->radius);
}

void printElf(elf *e) {
    printf("Name : %s\n", e->name);
    printf("Position : (%d, %d)\n", e->x, e->y);
    printf("HP : %d\n", e->hp);
    printf("Stamina : %d\n", e->stamina);
    printf("DMG : %d\n", e->dmg);
    printf("Won : %d\n", e->won);
}

void setGloves(elf *e, int gloves) { e->dmg = gloves; }

unsigned int getGloves(elf *e) { return e->dmg; }

void setStamina(elf *e, int stamina) { e->stamina = stamina; }

unsigned int getStamina(elf *e) { return e->stamina; }

void getPosition(elf *e, unsigned int *x, unsigned int *y) {
    *x = e->x;
    *y = e->y;
}

void takeGloves(elf *e, map *m) {
    // If he found better gloves, he swaps them
    printf("%d %d\n", e->dmg, getCellGloves(m, e->x, e->y));
    if (e->dmg < getCellGloves(m, e->x, e->y)) {
        unsigned int aux;
        aux = e->dmg;
        e->dmg = getCellGloves(m, e->x, e->y);
        setCellGloves(m, e->x, e->y, aux);
    }
}

void takesDamage(elf *e, unsigned int dmg) {
    if (dmg > e->hp) {
        e->hp = 0;
    } else {
        e->hp = e->hp - dmg;
    }
}

void setPosition(elf *e, unsigned int x, unsigned int y) {
    e->x = x;
    e->y = y;
}
