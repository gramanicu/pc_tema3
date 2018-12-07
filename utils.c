#include "utils.h"
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

void parseSnowstorm(int k, snowstorm *s) {
    s->dmg = getBit(k, 3);
    s->r = getBit(k, 2);
    s->y = getBit(k, 1);
    s->x = getBit(k, 0);
}

void createElf(elf *e, char *name, char x, char y, char hp, char stamina) {
    e->name = calloc(strlen(name), sizeof(char));
    strcpy(e->name, name);
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
    printf("Name : %s\n", e->name);
    printf("Position : (%d, %d)\n", e->x, e->y);
    printf("HP : %d\n", e->hp);
    printf("Stamina : %d\n", e->stamina);
    printf("DMG : %d\n", e->dmg);
}

char isInRadius(elf *e, char r) {
    int x = e->x;
    int y = e->y;
    double distance = sqrt(x * x + y * y);
    return distance <= r ? 1 : 0;
}

unsigned char getBit(int x, int n) { return (x >> (8 * n)) & 0xFF; }

void setCellHeight(map *m, int x, int y, char height) {
    m->cells[y * m->radius * m->radius + x].height = height;
}

void setCellGloves(map *m, int x, int y, char glove) {
    m->cells[y * m->radius * m->radius + x].glove = glove;
}

void setCell(map *m, int x, int y, char height, char glove) {
    setCellGloves(m, x, y, glove);
    setCellHeight(m, x, y, height);
}

char getCellHeight(map *m, int x, int y) {
    return m->cells[y * m->radius * m->radius + x].height;
}

char getCellGlove(map *m, int x, int y) {
    return m->cells[y * m->radius * m->radius + x].glove;
}

void setMapRadius(map *m, char radius) {
    prepareMap(m); 
    m->radius = radius; 
}

void prepareMap(map *m) {
    m->cells = calloc(pow((pow(m->radius, 2) + 1), 2), sizeof(cell));
}

int mapSize(map *m) {
    return 2 * pow(m->radius, 2) + 1;
}

int mapArea(map *m) {
    return pow(mapSize(m), 2) +1;
}

void printMapTopographic(map *m) {
    for (int i = 0; i < mapSize(m); i++) {
        for (int j = 0; j < mapSize(m); j++) {
            printf("%d ", m->cells[i * mapSize(m) + j].height);
        }
        printf("\n");
    }
}

void generateMap(map *m, char radius, FILE *in) {
    char height, glove;
    setMapRadius(m, radius);
    for (int i = 0; i <= mapSize(m); i++) {
        for (int j = 0; j <= mapSize(m); j++) {
            //setCell(m,j,i,height, glove);
        }
    }
}

void prepareGame(map *m) {
    FILE *in = fopen("date.in", "rt");
    char radius, playerCount;
    elf *players;

    fscanf(in, "%c", &radius);
    setMapRadius(m, radius);

    fscanf(in, "%c", &playerCount);
    players = calloc(playerCount, sizeof(elf));

    
    




}