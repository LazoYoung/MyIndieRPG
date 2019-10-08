#include <stdbool.h>
#include "game.h"

PlayerAttribute p_attr = {
    .agility = 1,
    .strength = 1,
    .health = 100,
    .mp = 100,
    .level = 1,
    .exp = 0,
    .skills = 0,
    .coin = 0,
    .name = "UNDEFINED"
};

void startGame(PlayerAttribute new_attr) {
    // TODO Implement
}

/* Assigns a given skill into the data */
void assignSkill(PlayerAttribute *data, short skill_code) {
    data -> skills |= (1 << skill_code);
}

/* Determines whether or not a given skill is achieved and available */
bool isSkillAvailable(short skill_code) {
    // TODO Implement
    return false;
}