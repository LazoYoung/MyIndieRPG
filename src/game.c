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
#include "header/vector.h"

PlayerProperty p_attr;
Inventory inv;
GItem item_reg[ITEMTYPE_SIZE];
bool inGame = false;
static Entity player;
const float deltaTime = 50 / 1000.0;
const int fps = 1000 / 50;

extern void updatePhysic(Entity*);
static void initPlayer();
static void onPlayerDeath(Entity*);

void startGame() {
    GItem sword1, sword2, sword3, armor1;

    setScreenMode(GAME_SCREEN);
    generateLevel(LOBBY);
    initPlayer();

    sword1.category = WEAPON;
    sword1.type = SMALL_SWORD;
    sword1.value = 5;
    sword1.equip = false;
    sword2.category = WEAPON;
    sword2.type = BRONZE_SWORD;
    sword2.value = 10;
    sword2.equip = false;
    sword3.category = WEAPON;
    sword3.type = STEEL_BLADE;
    sword3.value = 15;
    sword3.equip = false;
    armor1.category = ARMORY;
    armor1.type = HOOD_CAPE;
    armor1.value = 5;
    armor1.equip = false;
    item_reg[SMALL_SWORD] = sword1;
    item_reg[BRONZE_SWORD] = sword2;
    item_reg[STEEL_BLADE] = sword3;
    item_reg[HOOD_CAPE] = armor1;
    addItem(SMALL_SWORD);
    addItem(HOOD_CAPE);

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

void addItem(ItemType type) {
    for (int i = 0; i < INVENTORY_CAP; i++) {        
        if (inv.items[i] == NULL) {
            GItem *item = &item_reg[type];
            inv.items[i] = item;

            if (item->equip)
                inv.equipment[item->category] = item;
                
            break;
        }
    }
}

/**
 * Adds experience of player.
 * Returns true if he/she levels up.
 **/
bool addExp(int exp) {
    int rem;

    p_attr.exp += exp;
    rem = p_attr.exp - getExpCap(p_attr.level);

    if (rem >= 0) {
        p_attr.level++;
        p_attr.exp = rem;
        return true;
    }
    return false;
}

void doTick(int key) {
    if (!inGame)
        return;
    
    if (getStage() != VOID) {
        int id = 0;
        Entity* iter = getEntityByID(id++);

        while (iter) {
            if (iter->loc.pos[1] < 0) {
                iter->health--;
            }

            if (iter->health <= 0) {
                iter->deathEvent(iter);
                break;
            }

            if (iter->type == PLAYER) {
                updateControl(key, iter);
            }

            updatePhysic(iter);


            iter = getEntityByID(id++);
        }
    }
}

/* Returns the rounded count of frames being made during the given time-frame (ms) */
int getFramesDuringTime(int miliseconds) {
    return round(fps * miliseconds / 1000);
}

int getExpCap(int level) {
    return sqrt(level) * 100;
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

static void initPlayer() {
    AABB hitbox = {{0.0, 0.0}, {0.0, 0.0}};
    bool map[9][9] = {false};
    Texture skin;

    for (int i = 0; i < INVENTORY_CAP; i++) {
        inv.items[i] = NULL;
    }
    inv.skills = 0;
    inv.coin = 0;
    inv.equipment[0] = NULL;
    inv.equipment[1] = NULL;
    inv.equipment[2] = NULL;
    
    p_attr.mp = 100;
    p_attr.level = 1;
    p_attr.exp = 0;

    map[3][4] = map[4][4] = true;
    skin.color = COLOR_CYAN;
    memcpy(skin.map, map, sizeof(map));

    player.name = p_attr.name;
    player.type = PLAYER;
    player.loc = getTopLocation(5);
    player.target = NULL;
    player.hitbox = hitbox;
    player.health = p_attr.health;
    player.absorb = 0;
    player.damage = 1;
    player.agility = p_attr.agility;
    player.strength = p_attr.strength;
    player.offset[0] = 0.0;
    player.offset[1] = 0.0;
    player.skin = skin;
    player.deathEvent = onPlayerDeath;
    spawnEntity(&player);
}

static void onPlayerDeath(Entity* entity) {
    generateLevel(LOBBY);
    setPromptMode(DIALOGUE_PROMPT);
    mvwprintw(getPromptWindow(0), 3, 3, "You died! Respawning back to lobby...");

    entity->health = p_attr.health;
    entity->loc = getTopLocation(5);
}