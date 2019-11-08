// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncursesw/curses.h>
#include "header/game.h"
#include "header/data.h"
#include "header/level.h"
#include "header/physic.h"
#include "header/screen.h"
#include "header/vector.h"

Inventory inv;
bool inGame = false;
static Entity playerEntity;
extern Entity *entity[MAX_ENTITY];
const float deltaTime = 50 / 1000.0;
const int fps = 1000 / 50;

extern void updatePhysic(Entity*);
static void initInventory();
static void initGameCache();
static void onPlayerDeath(Entity*);

void startGame() {
    setScreenMode(GAME_SCREEN);
    generateLevel(LOBBY);
    initGameCache();
    spawnEntity(&playerEntity);

    inGame = true;
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

bool addItem(ItemType type) {
    int category = itemData[type][I_CATEGORY];

    for (int i = 0; i < SLOT_CAP; i++) {
        if (inv.items[category][i] < 0) {
            inv.items[category][i] = type;
            return true;
        }
    }

    return false;
}

/**
 * Adds experience of player.
 * Returns true if he/she levels up.
 **/
bool addExp(int exp) {
    int rem, *playerExp, *playerLevel;
    bool result = false;

    playerExp = &playerData[playerType][P_EXP];
    playerLevel = &playerData[playerType][P_LEVEL];
    *playerExp += exp;
    rem = *playerExp - getExpCap(*playerLevel);

    while (rem >= 0) {
        *playerLevel += 1;
        *playerExp = rem;        
        rem = *playerExp - getExpCap(*playerLevel);
        result = true;
    }

    return result;
}

bool removeItem(ItemType type) {
    ItemCategory category = itemData[type][I_CATEGORY];

    for (int i = 0; i < SLOT_CAP; i++) {
        int* sub = &inv.items[category][i];
        int* equip = &inv.equipment[category];

        if (type != *sub)
            continue;

        *sub = -1;

        if (type == *equip) {
            *equip = -1;
        }
        return true;
    }
    return false;
}

void doTick(int key) {
    if (!inGame)
        return;
    
    if (getStage() != VOID) {
        int id = 0;
        Entity* iter;

        while (id < MAX_ENTITY) {
            iter = getEntityByID(id++);

            if (!iter) continue;

            if (iter->loc.pos[1] < 0) {
                iter->health--;
            }

            if (iter->health <= 0) {
                iter->deathEvent(iter);
                break;
            }

            if (iter->type[0] == PLAYER) {
                static int regen = 40;

                if (regen-- == 0) {
                    if (iter->mp < playerData[playerType][P_MAX_MP])
                        iter->mp++;

                    regen = 40;
                }

                updateControl(key, iter);
            }

            updatePhysic(iter);
            updateSwordTrail();
        }
    }
}

int getExpCap(int level) {
    return floor(sqrt(level) * 100);
}

/* Returns the rounded count of frames being made during the given time-frame (ms) */
int getFramesDuringTime(int miliseconds) {
    return round(fps * miliseconds / 1000);
}

static void initGameCache() {
    AABB hitbox = {{0.0, 0.0}, {0.0, 0.0}};
    bool map[9][9] = {false};
    Texture skin;

    loadInventory();

    inv.skills = 0;
    inv.coin = 0;

    map[3][4] = map[4][4] = true;
    skin.color = COLOR_CYAN;
    memcpy(skin.map, map, sizeof(map));

    for (int i = 0; i < MAX_ENTITY; i++)
        entity[i] = NULL;

    playerEntity.name = getPlayerName(playerType);
    playerEntity.type[0] = PLAYER;
    playerEntity.type[1] = playerType;
    playerEntity.loc = getTopLocation(5);
    playerEntity.target = NULL;
    playerEntity.hitbox = hitbox;
    playerEntity.health = playerData[playerType][P_MAX_HEALTH];
    playerEntity.mp = playerData[playerType][P_MAX_MP];
    playerEntity.damage = 1;
    playerEntity.offset[0] = 0.0;
    playerEntity.offset[1] = 0.0;
    playerEntity.skin = skin;
    playerEntity.deathEvent = onPlayerDeath;
}

static void onPlayerDeath(Entity* entity) {
    generateLevel(LOBBY);
    setPromptMode(DIALOGUE_PROMPT);
    mvwprintw(getPromptWindow(0), 3, 3, "You died! Respawning back to lobby...");

    entity->health = playerData[playerType][P_MAX_HEALTH];
    entity->loc = getTopLocation(5);
}