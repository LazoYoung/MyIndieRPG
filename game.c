#undef _GNU_SOURCE
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdbool.h>
#include "game.h"

PlayerAttribute p_attr = {
    .agility = 1,
    .strength = 1,
    .health = 100,
    .mp = 100,
    .level = 1,
    .exp = 0,
    .name = "UNDEFINED"
};

Inventory inv = {
    .sword = NULL,
    .skills = 0,
    .coin = 0
};

void startGame() {
    // TODO Implement
}

/* Assigns a given skill into the player's inventory */
void assignSkill(char skill_code) {
    inv.skills |= (1 << skill_code);
}

/* Determines whether or not a given skill is achieved by the player */
bool hasSkill(char skill_code) {
    long long filter = 1 << skill_code;
    return (inv.skills & filter) == filter;
}