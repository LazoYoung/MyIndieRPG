// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdlib.h>
#include <stdbool.h>
#include "game.h"
#include "level.h"

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
    .item = NULL, // TODO dynamic allocation may be required for this array
    .skills = 0,
    .coin = 0
};

bool inGame = false;

float deltaTime = 50 / 1000.0;

void startGame() {
    inGame = true;

    spawnEntity(p_attr.name, getSpawnLocation(0));
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

bool doTick() {
    if (inGame) {
        updateEntities();
    }

    return true;
}