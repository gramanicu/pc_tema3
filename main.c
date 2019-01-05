// Copyright Grama Nicolae 2018

#include <stdio.h>
#include "./utils.h"

// I supposed an elf can't move more than 100 times at a time
#define MAX_DIRECTIONS 100

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

#define PRINT_ELVES_BOARD(...)                                             \
    fprintf(out, "%s \t %s \t %d\n", (players + id[i])->name, __VA_ARGS__, \
            eliminated[i]);

/*  It will prepare the game and "return" (using pointers)
    the map, players and player count, so we can deallocate the
    memory at the end of execution
*/
void prepareGame(map *m, elf **players, uint *count, FILE *in, FILE *out);

// Prints the leaderboard (count is the initial number of elves)
void printLeaderboard(elf *players, uint count, FILE *out);

// Frees all dinamically allocated memory
void releaseMemory(map *m, elf *players, uint playerCount, FILE *in, FILE *out);

// Reads the elves moves and the events (snowstorms, meltdowns ...)
void battle(map *m, elf **players, uint *count, FILE *in, FILE *out);

// Implementation for the MELTDOWN command
void meltdown(map *m, elf *players, uint staminaBonus, uint playerCount,
              FILE *in, FILE *out);

// Soaks an elf
void eliminateElf(map *m, elf *players, uint id, uint playerCount, FILE *in,
                  FILE *out);

// Implementation for the SNOWSTORM command
void startStorm(snowstorm *s, map *m, elf *players, uint playerCount, FILE *in,
                FILE *out);

// Check if the elf has fallen off the glacier
char isOut(elf *players, uint id, map *m);

// Check if there is another elf at the specified position (exeption is the id
// of the current elf
int playerAtPosition(elf *players, uint exeption, uint playerCount, uint x,
                     uint y);

// Starts the fight between two elves. If the one who got second into the
// cell won, the function returns 1. Else, it returns 0
int fight(elf *att, elf *def);

// Moves the player into a cell
int movePlayer(elf *players, map *m, uint id, uint *playerCount, char move,
               FILE *in, FILE *out);

// Reads player information from the file and spawns the elves
void spawnPlayers(elf *players, uint playerCount, FILE *in);

// Check if any player has missed the glacier
void checkLanding(elf *players, map *m, uint *playerCount, FILE *in, FILE *out);

// Check if the game has ended
int checkFinished(elf *players, uint playerCount, FILE *out);

int main() {
    map m;
    elf *players;
    uint playerCount;
    FILE *in, *out;

    prepareFiles(&in, &out);
    prepareGame(&m, &players, &playerCount, in, out);
    battle(&m, &players, &playerCount, in, out);

    releaseMemory(&m, players, playerCount, in, out);

    return 0;
}

void prepareGame(map *m, elf **players, uint *count, FILE *in, FILE *out) {
    uint radius, playerCount;

    fscanf(in, "%ud", &radius);
    fscanf(in, "%ud", &playerCount);

    generateMap(m, radius, radius, in);

    *players = calloc(playerCount, sizeof(elf));
    spawnPlayers(*players, playerCount, in);
    checkLanding(*players, m, &playerCount, in, out);

    *count = playerCount;
}

void spawnPlayers(elf *players, uint playerCount, FILE *in) {
    uint i, x, y, hp, stamina;
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

void battle(map *m, elf **players, uint *count, FILE *in, FILE *out) {
    char command[20];
    uint id = 0, i, stamina;
    int k;
    char directions[MAX_DIRECTIONS];

    while (fscanf(in, "%s", command) != EOF) {
        if (strcmp(command, "MOVE") == 0) {
            id = 0;
            fscanf(in, "%ud", &id);
            fscanf(in, "%s", directions);
            for (i = 0; i < strlen(directions); i++) {
                if (movePlayer(*players, m, id, count, directions[i], in,
                               out) == 1) {
                    break;
                }
            }
        } else if (strcmp(command, "SNOWSTORM") == 0) {
            fscanf(in, "%d", &k);
            snowstorm *storm = malloc(sizeof(snowstorm));
            parseSnowstorm(k, storm);

            startStorm(storm, m, *players, *count, in, out);

            free(storm);
        } else if (strcmp(command, "PRINT_SCOREBOARD") == 0) {
            printLeaderboard(*players, *count, out);
        } else if (strcmp(command, "MELTDOWN") == 0) {
            fscanf(in, "%ud", &stamina);
            meltdown(m, *players, stamina, *count, in, out);
        }
    }
}

void printLeaderboard(elf *players, uint count, FILE *out) {
    int status[count], eliminated[count], id[count], ok = 1, aux;
    uint i;
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
                    lint cname_length = strlen((players + id[i])->name);
                    lint nname_length = strlen((players + id[i + 1])->name);
                    char curr[cname_length], next[nname_length];
                    memcpy(curr, (players + id[i])->name,
                           cname_length * sizeof(char));
                    memcpy(next, (players + id[i + 1])->name,
                           nname_length * sizeof(char));
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
            PRINT_ELVES_BOARD("DRY");
        } else {
            PRINT_ELVES_BOARD("WET");
        }
    }
}

void releaseMemory(map *m, elf *players, uint playerCount, FILE *in,
                   FILE *out) {
    uint i;
    releaseMap(m);
    for (i = 0; i < playerCount; i++) {
        releaseElf(players + i);
    }
    free(players);
    fclose(in);
    fclose(out);
    exit(0);
}

void meltdown(map *m, elf *players, uint staminaBonus, uint playerCount,
              FILE *in, FILE *out) {
    uint i;
    reduceMap(m);

    for (i = 0; i < playerCount; i++) {
        if (!checkPosition(players + i, m)) {
            if ((players + i)->hp != 0) {
                fprintf(out, "%s got wet because of global warming.\n",
                        (players + i)->name);
                eliminateElf(m, players, i, playerCount, in, out);
            }
        } else {
            setStamina(players + i, getStamina(players + i) + staminaBonus);
        }
    }
}

// !!!!!!!!!!!!!!!!!! Move the ending conditions here !!!!!!!!!!!!!!!!!!!!!!
void eliminateElf(map *m, elf *players, uint id, uint playerCount, FILE *in,
                  FILE *out) {
    (players + id)->hp = 0;
    if (checkFinished(players, playerCount, out)) {
        releaseMemory(m, players, playerCount, in, out);
    }
}

// Returns 1 if the game is ended
int checkFinished(elf *players, uint playerCount, FILE *out) {
    int alive = 0;
    uint i, id = 0;
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

void startStorm(snowstorm *s, map *m, elf *players, uint playerCount, FILE *in,
                FILE *out) {
    uint i;
    for (i = 0; i < playerCount; i++) {
        uint x = (players + i)->x;
        uint y = (players + i)->y;
        double lenght = sqrt((x - s->x) * (x - s->x) + (y - s->y) * (y - s->y));
        if (lenght <= s->r) {
            if ((players + i)->hp != 0) {
                takesDamage(players + i, s->dmg);
                if ((players + i)->hp == 0) {
                    fprintf(out, "%s was hit by snowstorm.\n",
                            (players + i)->name);
                    eliminateElf(m, players, i, playerCount, in, out);
                }
            }
        }
    }
}

void checkLanding(elf *players, map *m, uint *playerCount, FILE *in,
                  FILE *out) {
    uint i;
    for (i = 0; i < *playerCount; i++) {
        // If he hasn't landed on the glacier
        if (!checkPosition(players + i, m)) {
            fprintf(out, "%s has missed the glacier.\n", (players + i)->name);
            eliminateElf(m, players, i, *playerCount, in, out);
        } else {
            takeGloves(players + i, m);
        }
    }
}

// Will return 1 if the elf was soaked
int movePlayer(elf *players, map *m, uint id, uint *playerCount, char move,
               FILE *in, FILE *out) {
    if ((players + id)->hp == 0) return 1;
    uint x, y, fallen = 0;
    int height;
    getPosition(players + id, &x, &y);
    switch (move) {
        // Will do the move only if he has enough stamina
        case 'U':
            height = getCellHeight(m, x - 1, y);
            if (height != -1) {
                if (getStamina(players + id) >= (uint)height) {
                    setPosition(players + id, x - 1, y);
                }
            } else {
                fallen = 1;
            }
            break;
        case 'D':
            height = getCellHeight(m, x + 1, y);
            if (height != -1) {
                if (getStamina(players + id) >= (uint)height) {
                    setPosition(players + id, x + 1, y);
                }
            } else {
                fallen = 1;
            }
            break;
        case 'R':
            height = getCellHeight(m, x, y + 1);
            if (height != -1) {
                if (getStamina(players + id) >= (uint)height) {
                    setPosition(players + id, x, y + 1);
                }
            } else {
                fallen = 1;
            }
            break;
        case 'L':
            height = getCellHeight(m, x, y - 1);
            if (height != -1) {
                if (getStamina(players + id) >= (uint)height) {
                    setPosition(players + id, x, y - 1);
                }
            } else {
                fallen = 1;
            }
            break;

            // Verifies if he fell out of the map
            if (!fallen) {
                // Consumes the stamina required to climb the hill
                uint ax, ay;
                getPosition(players + id, &ax, &ay);
                uint loss;
                loss = getCellHeight(m, x, y) - getCellHeight(m, ax, ay);
                setStamina(players + id, loss);
            }
    }

    if (isOut(players, id, m) || fallen) {
        fprintf(out, "%s fell of the glacier.\n", (players + id)->name);
        eliminateElf(m, players, id, *playerCount, in, out);
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
                    eliminateElf(m, players, defID, *playerCount, in, out);
                } else {
                    // he lost the fight
                    fprintf(out, "%s sent %s back home.\n",
                            (players + defID)->name, (players + id)->name);
                    (players + defID)->stamina = (players + id)->stamina;
                    (players + defID)->won = (players + defID)->won + 1;
                    eliminateElf(m, players, id, *playerCount, in, out);
                }
            }
        }
    }
    return 0;
}

int playerAtPosition(elf *players, uint exeption, uint playerCount, uint x,
                     uint y) {
    uint i;
    for (i = 0; i < playerCount; i++) {
        if (i != exeption) {
            uint ex, ey;
            getPosition(players + i, &ex, &ey);
            if (ex == x && ey == y) {
                return i;
            }
        }
    }
    return -1;
}

char isOut(elf *players, uint id, map *m) {
    if (!checkPosition(players + id, m)) {
        return 1;
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
