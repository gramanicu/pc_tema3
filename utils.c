#include "utils.h"
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "elves.h"

char isInRadius(elf *e, unsigned int r) {
    int x = e->x;
    int y = e->y;
    int lat = 2*r+1;
    return (x>=0 && y>=0 && x<lat && y<lat) ? 1 : 0;
}

unsigned char getBit(int x, int n) { return (x >> (8 * n)) & 0xFF; }

void prepareFiles(FILE **in, FILE **out, char *filename) {
    char *inputFile = calloc(strlen(filename) + 4, sizeof(char));
    char *outputFile = calloc(strlen(filename) + 5, sizeof(char));
    strcat(inputFile, filename);
    strcat(outputFile, filename);
    strcat(inputFile, ".in");
    strcat(outputFile, ".out");

    *in = fopen(inputFile, "rt");
    *out = fopen(outputFile, "w");
    free(inputFile);
    free(outputFile);
}
