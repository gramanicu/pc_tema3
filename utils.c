#include "utils.h"
#include <math.h>
#include <stdio.h>

void parseSnowstorm(int k, snowstorm *s) {
    s->dmg = getBit(k, 3);
    s->r = getBit(k, 2);
    s->y = getBit(k, 1);
    s->x = getBit(k, 0);
}

void createElf(elf *e, int x, int y, int hp, int stamina) {
    e->x = x;
    e->y = y;
    e->hp = hp;
    e->stamina = stamina;
    e->dmg = 0;
}

void printSnowstorm(snowstorm *s) {
    printf("Damage = %d\n", s->dmg);
    printf("Radius = %d\n", s->r);
    printf("Y = %d\n", s->y);
    printf("X = %d\n", s->x);
}

void printElf(elf *e) {
    printf("Position : (%d, %d)\n", e->x, e->y);
    printf("HP : %d\n", e->hp);
    printf("Stamina : %d\n", e->stamina);
    printf("DMG : %d\n", e->dmg);
}

char isInRadius(elf *e, int r) {
    int x = e->x;
    int y = e->y;
    double distance = sqrt(x * x + y * y);
    return distance <= r ? 1 : 0;
}

unsigned char getBit(int x, int n) { return (x >> (8 * n)) & 0xFF; }