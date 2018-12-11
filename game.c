#include "game.h"
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elves.h"
#include "map.h"

// I supposed an elf can't move more than 100 times at a time
#define MAX_DIRECTIONS 100

#define MAX_NAME_LENGTH 30

// This is used so i don't repeat this block 3 times in the
// print leaderboard function
#define SWITCH_ELVES()                     \
    {                                      \
        aux = id[i];                       \
        id[i] = id[i + 1];                 \
        id[i + 1] = aux;                   \
        aux = eliminated[i];               \
        eliminated[i] = eliminated[i + 1]; \
        eliminated[i + 1] = aux;           \
        aux = status[i];                   \
        status[i] = status[i + 1];         \
        status[i + 1] = aux;               \
        ok = 1;                            \
    }

void startGame(char *files) {
    map m;
    elf *players;
    unsigned int playerCount;

    FILE *in, *out;

    prepareFiles(&in, &out, files);
    prepareGame(&m, &players, &playerCount, in, out);
    battle(&m, &players, &playerCount, in, out);

    // printPlayers(players, playerCount);
    // printHeightmap(&m);
    releaseMemory(&m, players, playerCount, in, out);
}

void printLeaderboard(elf *players, unsigned int count, FILE *out) {
    int status[count], eliminated[count], id[count], ok = 1, aux;
    unsigned int i;
    for (i = 0; i < count; i++) {
        id[i] = i;
        eliminated[i] = (players + i)->won;
        status[i] = ((players + i)->hp != 0 ? 1 : 0);
    }

    while (ok) {
        ok = 0;
        for (i = 0; i < count - 1; i++) {
            if (status[i] < status[i + 1]) {
                SWITCH_ELVES();
            } else if (status[i] == status[i + 1]) {
                if (eliminated[i] < eliminated[i + 1]) {
                    SWITCH_ELVES();
                } else if (eliminated[i] == eliminated[i + 1]) {
                    char curr[MAX_NAME_LENGTH], next[MAX_NAME_LENGTH];
                    strcpy(curr, (players + id[i])->name);
                    strcpy(next, (players + id[i + 1])->name);
                    if (strcmp(curr, next) > 0) {
                        SWITCH_ELVES();
                    }
                }
            }
        }
    }

    fprintf(out, "SCOREBOARD:\n");
    for (i = 0; i < count; i++) {
        if (status[i]) {
            fprintf(out, "%s \t %s \t %d\n", (players + id[i])->name, "DRY",
                    eliminated[i]);
        } else {
            fprintf(out, "%s \t %s \t %d\n", (players + id[i])->name, "WET",
                    eliminated[i]);
        }
    }
}

void battle(map *m, elf **players, unsigned int *count, FILE *in, FILE *out) {
    char command[20];
    unsigned int id = 0, i, stamina;
    int k, cont;
    char directions[MAX_DIRECTIONS];

    while (fscanf(in, "%s", command) != EOF) {
        if (strcmp(command, "MOVE") == 0) {
            id = 0;
            fscanf(in, "%ud", &id);
            fscanf(in, "%s", directions);
            for (i = 0; i < strlen(directions); i++) {
                cont = movePlayer(*players, m, id, count, directions[i], out);
                if (cont == 1) {
                    return;
                } else if (cont == -1) {
                    if(checkFinished(*players, *count, out)) {
                        break;
                    }
                }
            }
        } else if (strcmp(command, "SNOWSTORM") == 0) {
            fscanf(in, "%d", &k);
            snowstorm *storm = malloc(sizeof(snowstorm));
            parseSnowstorm(k, storm);

            if (startStorm(storm, *players, *count, out)) {
                free(storm);
                return;
            }
            free(storm);
        } else if (strcmp(command, "PRINT_SCOREBOARD") == 0) {
            printLeaderboard(*players, *count, out);
        } else if (strcmp(command, "MELTDOWN") == 0) {
            fscanf(in, "%ud", &stamina);
            if (meltdown(m, *players, stamina, *count, out)) {
                return;
            }
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
        printf("\n");
        printElf(players + i);
    }
    printf("\n");
}

// Return 1 if game ended
int checkLanding(elf *players, unsigned int *playerCount, map *m, FILE *out) {
    unsigned int i;
    for (i = 0; i < *playerCount; i++) {
        // If he hasn't landed on the glacier
        if (!checkPosition(players + i, m)) {
            fprintf(out, "%s has missed the glacier.\n", (players + i)->name);
            eliminateElf(players, i);

            /*  Check if the game is finished (the funny situation when
                all players except the last one to land have missed the
                glacier - he could have missed it too, still winner :)
            */
            if (checkFinished(players, *playerCount, out)) {
                return 1;
            }
        } else {
            takeGloves(players + i, m);
        }
    }
    return 0;
}

void eliminateElf(elf *players, unsigned int id) {
    // changed eliminate function because of a problem specification
    (players + id)->hp = 0;
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

int checkFinished(elf *players, unsigned int playerCount, FILE *out) {
    int alive = 0;
    unsigned int i, id;
    for (i = 0; i < playerCount; i++) {
        if ((players + i)->hp > 0) {
            alive++;
            id = i;
        }
    }
    if (alive == 1) {
        fprintf(out, "%s has won!\n", (players + id)->name);
        return 1;
    } else {
        return 0;
    }
}

// Will return 1 if the game is over and -1 if the elf was "soaked"
int movePlayer(elf *players, map *m, unsigned int id, unsigned int *playerCount,
               char move, FILE *out) {
    if ((players + id)->hp == 0) return -1;
    unsigned int x, y, fallen = 0;
    int height;
    getPosition(players + id, &x, &y);
    switch (move) {
        // Will do the move only if he has enough stamina
        case 'U':
            height = getCellHeight(m, x - 1, y);
            if (height != -1) {
                if (getStamina(players + id) >= (unsigned int)height) {
                    setPosition(players + id, x - 1, y);
                }
            } else {
                fallen = 1;
            }
            break;
        case 'D':
            height = getCellHeight(m, x + 1, y);
            if (height != -1) {
                if (getStamina(players + id) >= (unsigned int)height) {
                    setPosition(players + id, x + 1, y);
                }
            } else {
                fallen = 1;
            }
            break;
        case 'R':
            height = getCellHeight(m, x, y + 1);
            if (height != -1) {
                if (getStamina(players + id) >= (unsigned int)height) {
                    setPosition(players + id, x, y + 1);
                }
            } else {
                fallen = 1;
            }
            break;
        case 'L':
            height = getCellHeight(m, x, y - 1);
            if (height != -1) {
                if (getStamina(players + id) >= (unsigned int)height) {
                    setPosition(players + id, x, y - 1);
                }
            } else {
                fallen = 1;
            }
            break;

            // Verifies if he fell out of the map
            if (!fallen) {
                // Consumes the stamina required to climb the hill
                unsigned int ax, ay;
                getPosition(players + id, &ax, &ay);
                unsigned int loss;
                loss = getCellHeight(m, x, y) - getCellHeight(m, ax, ay);
                setStamina(players + id, loss);
            }
    }

    if (isOut(players, id, m) || fallen) {
        fprintf(out, "%s fell of the glacier.\n", (players + id)->name);
        eliminateElf(players, id);
        return checkFinished(players, *playerCount, out);
    } else {
        takeGloves(players + id, m);

        // Check if there is another player at his position
        int defID = playerAtPosition(players, id, *playerCount,
                                     (players + id)->x, (players + id)->y);
        if (defID != -1) {
            if ((players + defID)->hp != 0) {
                if (fight(players + id, players + defID)) {
                    // he won the fight
                    fprintf(out, "%s sent %s back home.\n",
                            (players + id)->name, (players + defID)->name);
                    (players + id)->stamina = (players + defID)->stamina;
                    (players + id)->won = (players + id)->won + 1;
                    eliminateElf(players, defID);
                } else {
                    // he lost the fight
                    fprintf(out, "%s sent %s back home.\n",
                            (players + defID)->name, (players + id)->name);
                    (players + defID)->stamina = (players + id)->stamina;
                    (players + defID)->won = (players + defID)->won + 1;
                    eliminateElf(players, id);
                    return -1;
                }
            }
        }

        return checkFinished(players, *playerCount, out);
    }
    return 0;
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
            if (def->hp == 0) {
                return 1;
            }
            turn = 0;
        } else {
            takesDamage(att, def->dmg);
            if (att->hp == 0) {
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

// Return 1 if game ended
int meltdown(map *m, elf *players, unsigned int staminaBonus,
             unsigned int playerCount, FILE *out) {
    unsigned int i;
    m->radius = m->radius - 1;
    m->diameter = 2 * m->diameter + 1;
    for (i = 0; i < playerCount; i++) {
        if (!checkPosition(players + i, m)) {
            if ((players + i)->hp != 0) {
                fprintf(out, "%s got wet because of global warming.\n",
                        (players + i)->name);
                eliminateElf(players, i);
                if (checkFinished(players, playerCount, out)) {
                    return 1;
                }
            }
        } else {
            setStamina(players + i, getStamina(players + i) + staminaBonus);
        }
    }

    return 0;
}

// Returns 1 if game ended
int startStorm(snowstorm *s, elf *players, unsigned int playerCount,
               FILE *out) {
    unsigned int i;
    for (i = 0; i < playerCount; i++) {
        unsigned int x = (players + i)->x;
        unsigned int y = (players + i)->y;
        double lenght = sqrt((x - s->x) * (x - s->x) + (y - s->y) * (y - s->y));
        if (lenght <= s->r) {
            if ((players + i)->hp != 0) {
                takesDamage(players + i, s->dmg);
                if ((players + i)->hp == 0) {
                    fprintf(out, "%s was hit by snowstorm.\n",
                            (players + i)->name);
                    eliminateElf(players, i);
                    if (checkFinished(players, playerCount, out)) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}