// Copyright Grama Nicolae 2018
#include "utils.h"
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "elves.h"

char isInRadius(elf *e, unsigned int c, unsigned int r) {
    unsigned int x = e->x;
    unsigned int y = e->y;
    double lenght = sqrt((x - c) * (x - c) + (y - c) * (y - c));
    return lenght <= r ? 1 : 0;
}

unsigned char getBit(int x, int n) { return (x >> (8 * n)) & 0xFF; }

void prepareFiles(FILE **in, FILE **out, char *filename) {
    long int inSize = (strlen(filename) + 4) * sizeof(char);
    long int outSize = (strlen(filename) + 5) * sizeof(char);
    char *inputFile = malloc(inSize);
    char *outputFile = malloc(outSize);
    memcpy(inputFile, filename, strlen(filename) * sizeof(char));
    memcpy(outputFile, filename, strlen(filename) * sizeof(char));
    memcpy(inputFile + strlen(filename) * sizeof(char) + 1, ".in",
           4 * sizeof(char));
    memcpy(outputFile + strlen(filename) * sizeof(char) + 1, ".out",
           5 * sizeof(char));

    *in = fopen(inputFile, "rt");
    *out = fopen(outputFile, "w");
    free(inputFile);
    free(outputFile);
}
