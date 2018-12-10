#include "game.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elves.h"
#include "map.h"

// I supposed an elf can't move more than 100 times at a time
#define MAX_DIRECTIONS 100

void startGame(char *files) {
    map m;
    elf *players;
    unsigned int playerCount;

    FILE *in, *out;

    prepareFiles(&in, &out, files);
    prepareGame(&m, &players, &playerCount, in, out);

    battle(&m, &players, &playerCount, in, out);

    releaseMemory(&m, players, playerCount, in, out);
}

void battle(map *m, elf **players, unsigned int *count, FILE *in, FILE *out) {
    char command[20];
    while (fscanf(in, "%s", command) != EOF) {
        if (strcmp(command, "MOVE")) {
            unsigned int id = 0, i;
            char directions[MAX_DIRECTIONS];

            fscanf(in, "%ud", &id);
            fscanf(in, "%s", directions);

            for (i = 0; i < strlen(directions); i++) {
                // printf("%d %c\n", id, directions[i]);
                // movePlayer(players, m, id, &count, directions[i], out);
            }
        } else if (strcmp(command, "SNOWSTORM")) {
            int k;
            fscanf(in, "%d", &k);
        } else if (strcmp(command, "PRINT_SCOREBOARD")) {
        } else if (strcmp(command, "MELTDOWN")) {
            unsigned int stamina;
            fscanf(in, "%ud", &stamina);
        }
    }
}

// Reads the input to prepare the game
void prepareGame(map *m, elf **players, unsigned int *count, FILE *in,
                 FILE *out) {
    unsigned int radius, playerCount;

    fscanf(in, "%ud", &radius);
    fscanf(in, "%ud", &playerCount);

    generateMap(m, radius, in);
    *players = calloc(playerCount, sizeof(elf));
    spawnPlayers(*players, playerCount, in);
    checkLanding(*players, &playerCount, m, out);

    *count = playerCount;

    // printPlayers(*players, playerCount);
    // printHeightmap(m);
}

void spawnPlayers(elf *players, unsigned int playerCount, FILE *in) {
    unsigned int i, x, y, hp, stamina;
    char name[50];
    for (i = 0; i < playerCount; i++) {
        fscanf(in, "%s", name);
        fscanf(in, "%ud", &x);
        fscanf(in, "%ud", &y);
        fscanf(in, "%ud", &hp);
        fscanf(in, "%ud", &stamina);
        createElf(players + i, name, x, y, hp, stamina);
    }
}

void printPlayers(elf *players, unsigned int playerCount) {
    unsigned int i;
    for (i = 0; i < playerCount; i++) {
        printElf(players + i);
    }
}

void checkLanding(elf *players, unsigned int *playerCount, map *m, FILE *out) {
    unsigned int i;
    for (i = 0; i < *playerCount; i++) {
        // If he hasn't landed on the glacier
        if (!checkPosition(players + i, m)) {
            fprintf(out, "%s has missed the glacier.\n", (players + i)->name);
            eliminateElf(players, i, playerCount);

            /*  Check if the game is finished (the funny situation when
                all players except the last one to land have missed the
                glacier - he could have missed it too, still winner :)
            */
            checkFinished(players, *playerCount, out);
        }
    }
}

void eliminateElf(elf *players, unsigned int id, unsigned int *count) {
    unsigned int i;
    for (i = id; i < *count - 1; i++) {
        elf *aux = (players + i + 1);
        releaseElf(players + i);
        createElf(players + i, aux->name, aux->x, aux->y, aux->hp,
                  aux->stamina);
    }
    releaseElf(players + *count - 1);
    *count = *count - 1;
}

void releaseMemory(map *m, elf *players, unsigned int playerCount, FILE *in,
                   FILE *out) {
    unsigned int i;
    releaseMap(m);
    for (i = 0; i < playerCount; i++) {
        releaseElf(players + i);
    }
    free(players);
    fclose(in);
    fclose(out);
}

void checkFinished(elf *players, unsigned int playerCount, FILE *out) {
    if (playerCount == 1) {
        fprintf(out, "%s has won!\n", players->name);
        exit(0);
    }
}

void movePlayer(elf *players, map *m, unsigned int id,
                unsigned int *playerCount, char move, FILE *out) {
    unsigned int x, y;
    getPosition(players + id, &x, &y);
    switch (move) {
        // Will do the move only if he has enough stamina
        case 'U':
            if (getStamina(players + id) <= getCellHeight(m, x - 1, y)) {
                setPosition(players + id, x - 1, y);
            }
            break;
        case 'D':
            if (getStamina(players + id) <= getCellHeight(m, x + 1, y)) {
                setPosition(players + id, x + 1, y);
            }
            break;
        case 'R':
            if (getStamina(players + id) <= getCellHeight(m, x, y + 1)) {
                setPosition(players + id, x, y + 1);
            }
            break;
        case 'L':
            if (getStamina(players + id) <= getCellHeight(m, x, y - 1)) {
                setPosition(players + id, x, y - 1);
            }
            break;

            // Consumes the stamina required to climb the hill
            unsigned int ax, ay;
            getPosition(players + id, &ax, &ay);
            unsigned int loss;
            loss = getCellHeight(m, x, y) - getCellHeight(m, ax, ay);
            setStamina(players + id, loss);
    }

    if (isOut(players, id, m)) {
        fprintf(out, "%s fell of the glacier.\n", (players + id)->name);
        eliminateElf(players, id, playerCount);
        checkFinished(players, *playerCount, out);
    } else {
        takeGloves(players + id, m);

        // Finds the id of the player at the specified position
        int defID = playerAtPosition(players, id, *playerCount,
                                     (players + id)->x, (players + id)->y);

        // Checks if there is a player there
        if (defID != -1) {
            if (fight(players + id, players + defID)) {
                // he won the fight
                fprintf(out, "%s sent %s back home.\n", (players + id)->name,
                        (players + defID)->name);
                eliminateElf(players, defID, playerCount);
                checkFinished(players, *playerCount, out);
            } else {
                // he lost the fight
                fprintf(out, "%s sent %s back home.\n", (players + defID)->name,
                        (players + id)->name);
                eliminateElf(players, id, playerCount);
                checkFinished(players, *playerCount, out);
            }
        }
    }
}

int fight(elf *att, elf *def) {
    int turn;

    /*  if the one who got second in the specified cell has more or equal
       stamina, he attacks first
    */
    if (att->stamina >= def->stamina) {
        turn = 1;
    } else {
        turn = 0;
    }

    // elves attack each other untill someone is "Soaked"
    while (1) {
        if (turn) {
            takesDamage(def, att->dmg);
            if (!def->hp) {
                return 1;
            }
            turn = 0;
        } else {
            takesDamage(att, def->dmg);
            if (!att->hp) {
                return 0;
            }
            turn = 1;
        }
    }
}

char isOut(elf *players, unsigned int id, map *m) {
    if (!checkPosition(players + id, m)) {
        return 1;
    }
    return 0;
}

int playerAtPosition(elf *players, unsigned int exeption,
                     unsigned int playerCount, unsigned int x, unsigned int y) {
    unsigned int i;
    for (i = 0; i < playerCount; i++) {
        if (i != exeption) {
            unsigned int ex, ey;
            getPosition(players + i, &ex, &ey);
            if (ex == x && ey == y) {
                return i;
            }
        }
    }
    return -1;
}