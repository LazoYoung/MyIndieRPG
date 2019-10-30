// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncursesw/curses.h>
#include "header/game.h"
#include "header/level.h"
#include "header/physic.h"
#include "header/screen.h"

PlayerProperty p_attr = {
    .agility = 1,
    .strength = 1,
    .health = 100,
    .mp = 100,
    .level = 1,
    .exp = 0,
    .name = "UNDEFINED"
};
Inventory inv = {
    .items = { false }, // TODO dynamic array
    .equipment = NULL,
    .skills = 0,
    .coin = 0
};
static Entity player;
bool inGame = false;
const float deltaTime = 50 / 1000.0;
const int fps = 1000 / 50;

void startGame() {
    AABB hitbox = {{0.0, 0.0}, {0.0, 0.0}};
    Bias bias = {false, false, false, 0};
    bool map[9][9] = {false};
    Texture skin;
    GItem basic_sword, basic_armor;

    setStage(LOBBY);
    setScreen(GAME_SCREEN);

    map[3][4] = map[4][4] = true;
    skin.color = COLOR_CYAN;
    memcpy(skin.map, map, sizeof(map));
    player.valid = true;
    player.name = p_attr.name;
    player.loc = getSpawnLocation();
    player.hitbox = hitbox;
    player.offset[0] = 0.0;
    player.offset[1] = 0.0;
    player.skin = skin;
    player.bias = bias;
    inGame = true;
    spawnEntity(&player);

    basic_sword.valid = true;
    basic_sword.category = WEAPON;
    basic_sword.equip = true;
    basic_sword.type = SMALL_SWORD;
    basic_armor.valid = true;
    basic_armor.category = ARMORY;
    basic_armor.equip = true;
    basic_armor.type = HOOD_CAPE;
    addItem(basic_sword);
    addItem(basic_armor);
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

void addItem(GItem item) {
    for (int i=0; i<10; i++) {
        GItem *slot = &inv.items[i];
        
        if (!slot->valid) {
            slot->valid = true;
            slot->type = item.type;
            slot->category = item.category;
            slot->equip = item.equip;
            inv.equipment[item.category] = slot;
            break;
        }
    }
}

void doTick(int key) {
    if (!inGame)
        return;
    
    if (getStage() != VOID) {
        if (player.valid) {
            updateControl(key, &player.bias);
        }

        updateEntities();
    }
}

/* Returns the rounded count of frames being made during the given time-frame (ms) */
int getFramesDuringTime(int miliseconds) {
    return round(fps * miliseconds / 1000);
}

const char* getItemName(ItemType type) {
    switch (type) {
        case SMALL_SWORD:
            return "Small Sword";
        case BRONZE_SWORD:
            return "Bronze Sword";
        case STEEL_BLADE:
            return "Steel Blade";
        case HOOD_CAPE:
            return "Hood Cape";
        default:
            return NULL;
    }
}