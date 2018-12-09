#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "elves.h"
#include "game.h"
#include "map.h"

int main(int argc, char *argv[] ) {
    // nu au fost dat numele fisierului de intrare
    if(argc==1) exit(0);
    startGame(argv[1]);
    
    return 0;
}
