#include "map.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

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

void setCellHeight(map *m, int x, int y, unsigned int height) {
    m->cells[y * mapDiameter(m) + x].height = height;
}

void setCellGloves(map *m, int x, int y, char glove) {
    m->cells[y * mapDiameter(m) + x].glove = glove;
}

void setCell(map *m, int x, int y, unsigned int height, char glove) {
    setCellGloves(m, x, y, glove);
    setCellHeight(m, x, y, height);
}

unsigned int getCellHeight(map *m, int x, int y) {
    return m->cells[y * mapDiameter(m) + x].height;
}

unsigned int getCellGlove(map *m, int x, int y) {
    return m->cells[y * mapDiameter(m) + x].glove;
}

void prepareMap(map *m, unsigned int radius) {
    m->cells = calloc(pow(2 * radius + 1, 2), sizeof(cell));
    m->diameter = 2 * radius + 1;
}

void releaseMap(map *m) { free(m->cells); }

unsigned int mapDiameter(map *m) { return m->diameter; }

unsigned int mapArea(map *m) { return pow(mapDiameter(m), 2) + 1; }

void generateMap(map *m, unsigned int radius, FILE *in) {
    unsigned int i, j, height = 0, glove = 0;
    prepareMap(m, radius);
    for (i = 0; i < mapDiameter(m); i++) {
        for (j = 0; j < mapDiameter(m); j++) {
            fscanf(in, "%ud", &height);
            fscanf(in, "%ud", &glove);
            setCell(m, j, i, height, glove);
        }
    }
}

void printHeightmap(map *m) {
    unsigned int i, j;
    for (i = 0; i < mapDiameter(m); i++) {
        for (j = 0; j < mapDiameter(m); j++) {
            printf("%d ", m->cells[i * mapDiameter(m) + j].height);
        }
        printf("\n");
    }
}