// Copyright Grama Nicolae 2018
#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include "elves.h"

#define MAX_SIZE 100

// Return the n'th bit of the number x
unsigned char getBit(int x, int n);

// Check if a elf is inside a circle of radius r,
// centered in (c, c), using the pythagorean theorem
char isInRadius(elf *e, unsigned int c, unsigned int r);

void prepareFiles(FILE **in, FILE **out, char *filename);

#endif