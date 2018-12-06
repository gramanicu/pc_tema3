#include <stdio.h>
#include "utils.h"

void developTest() {
    snowstorm s;
    int x = 65536;
    parseSnowstorm(x, &s);
    printSnowstorm(&s);

    int a = 0, b = 100;
    elf john;
    createElf(&john, a, b, 100, 100);
    printf("\n(%d,%d) is in radius: %d\n\n", a, b, isInRadius(&john, 100));
    printElf(&john);
}

int main() {
    developTest();
    return 0;
}
