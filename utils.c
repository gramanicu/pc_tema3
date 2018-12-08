#include "utils.h"
#include "elves.h"
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

char isInRadius(elf *e, unsigned int r) {
    int x = e->x;
    int y = e->y;
    double distance = sqrt(x * x + y * y);
    return distance <= r ? 1 : 0;
}

unsigned char getBit(int x, int n) { return (x >> (8 * n)) & 0xFF; }

