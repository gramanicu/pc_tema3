#ifndef UTILS_H
#define UTILS_H
#include "elves.h"
#include <stdio.h>

#define MAX_SIZE 100

// Return the n'th bit of the number x
unsigned char getBit(int x, int n);

// Check if a elf is inside a circle of radius r,
// centered in (0, 0), using the pythagorean theorem
char isInRadius(elf *e, unsigned int r);

void prepareFiles(FILE **in, FILE **out,char *filename);

#endif