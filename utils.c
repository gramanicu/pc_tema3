// Copyright Grama Nicolae 2018
#include "./utils.h"

char isInRadius(elf *e, uint center, uint radius) {
    double x = (double)e->x;
    double y = (double)e->y;
    double c = (double)center;
    double r = (double)radius;

    double lenght = sqrt((x - c) * (x - c) + (y - c) * (y - c));
    if (strcmp(e->name, "Arturo") == 0) printf("%lf, %lf\n", lenght, r);
    return lenght <= r ? 1 : 0;
}

unsigned char getBit(int x, int n) { return (x >> (8 * n)) & 0xFF; }

void prepareFiles(FILE **in, FILE **out) {
    *in = fopen("snowfight.in", "rt");
    *out = fopen("snowfight.out", "w");
}

// -------------- Elves Region -----------------

void createElf(elf *e, char *name, int x, int y, int hp, int stamina) {
    e->name = calloc(strlen(name) + 1, sizeof(char));
    memcpy(e->name, name, (strlen(name) + 1) * sizeof(char));
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

uint getGloves(elf *e) { return e->dmg; }

void setStamina(elf *e, int stamina) { e->stamina = stamina; }

uint getStamina(elf *e) { return e->stamina; }

void getPosition(elf *e, uint *x, uint *y) {
    *x = e->x;
    *y = e->y;
}

void takeGloves(elf *e, map *m) {
    // If he found better gloves, he swaps them
    if (e->dmg < getCellGloves(m, e->x, e->y)) {
        uint aux;
        aux = e->dmg;
        e->dmg = getCellGloves(m, e->x, e->y);
        setCellGloves(m, e->x, e->y, aux);
    }
}

void takesDamage(elf *e, uint dmg) {
    if (dmg > e->hp) {
        e->hp = 0;
    } else {
        e->hp = e->hp - dmg;
    }
}

void setPosition(elf *e, uint x, uint y) {
    e->x = x;
    e->y = y;
}

// -------------------------------------------

// -------------- Map Region -----------------

void parseSnowstorm(int k, snowstorm *s) {
    s->dmg = getBit(k, 3);
    s->r = getBit(k, 2);
    s->y = getBit(k, 1);
    s->x = getBit(k, 0);
}

void printSnowstorm(snowstorm *s) {
    printf("Damage = %d\n", s->dmg);
    printf("Radius = %d\n", s->r);
    printf("Y = %d\n", s->y);
    printf("X = %d\n", s->x);
}

void setCellHeight(map *m, int x, int y, uint height) {
    (*accessCell(m, x, y)).height = height;
}

void setCellGloves(map *m, int x, int y, uint glove) {
    (*accessCell(m, x, y)).glove = glove;
}

void setCell(map *m, int x, int y, uint height, uint glove) {
    setCellGloves(m, x, y, glove);
    setCellHeight(m, x, y, height);
}

cell *accessCell(map *m, int x, int y) {
    return &(m->cells[(x - meltdownsCounter(m)) * mapOriginalDiameter(m) + y -
                      meltdownsCounter(m)]);
}

int getCellHeight(map *m, int x, int y) {
    if ((uint)x >= (m->center) * 2 + 1 || (uint)y >= (m->center) * 2 + 1 ||
        x < 0 || y < 0) {
        return -1;
    }
    return (*accessCell(m, x, y)).height;
}

uint getCellGloves(map *m, int x, int y) {
    return (*accessCell(m, x, y)).glove;
}

void prepareMap(map *m, uint radius, uint center) {
    m->cells = calloc(pow(2 * radius + 1, 2), sizeof(cell));
    m->diameter = 2 * radius + 1;
    m->radius = radius;
    m->center = center;
}

void releaseMap(map *m) { free(m->cells); }

uint mapDiameter(map *m) { return m->diameter; }

uint mapOriginalDiameter(map *m) { return (m->center) * 2 + 1; }

uint mapArea(map *m) { return pow(mapDiameter(m), 2) + 1; }

uint meltdownsCounter(map *m) { return m->center - m->radius; }

void generateMap(map *m, uint radius, uint center, FILE *in) {
    uint i, j, height = 0, glove = 0;
    prepareMap(m, radius, center);
    for (i = 0; i < mapDiameter(m); i++) {
        for (j = 0; j < mapDiameter(m); j++) {
            fscanf(in, "%ud", &height);
            fscanf(in, "%ud", &glove);
            setCell(m, i, j, height, glove);
        }
    }
}

void reduceMap(map *origin) {
    uint oldDiameter = origin->diameter;
    origin->radius = origin->radius - 1;
    origin->diameter = 2 * origin->radius + 1;
    uint i, j, a = 0, b = 0;

    // Transfer the data
    for (i = 1; i < oldDiameter - 1; i++) {
        for (j = 1; j < oldDiameter - 1; j++) {
            origin->cells[a * oldDiameter + b].height =
                origin->cells[i * oldDiameter + j].height;
            origin->cells[a * oldDiameter + b].glove =
                origin->cells[i * oldDiameter + j].glove;
            b++;
        }
        b = 0;
        a++;
    }
    a = 0;
}

void printHeightmap(map *m) {
    uint i, j;
    for (i = 0; i < mapDiameter(m); i++) {
        for (j = 0; j < mapDiameter(m); j++) {
            printf("%d ", m->cells[i * mapOriginalDiameter(m) + j].height);
        }
        printf("\n");
    }
}

void printGlovemap(map *m) {
    uint i, j;
    for (i = 0; i < mapDiameter(m); i++) {
        for (j = 0; j < mapDiameter(m); j++) {
            printf("%d ", m->cells[i * mapOriginalDiameter(m) + j].glove);
        }
        printf("\n");
    }
}

// -------------------------------------------
