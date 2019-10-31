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

PlayerProperty p_attr = {
    .agility = 1,
    .strength = 1,
    .mp = 100,
    .level = 1,
    .exp = 0,
    .name = NULL
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

extern void updatePhysic(Entity*);
static void onPlayerDeath(Entity*);

void startGame() {
    AABB hitbox = {{0.0, 0.0}, {0.0, 0.0}};
    bool map[9][9] = {false};
    Texture skin;
    GItem basic_sword, basic_armor;

    generateLevel(LOBBY);
    setScreenMode(GAME_SCREEN);

    map[3][4] = map[4][4] = true;
    skin.color = COLOR_CYAN;
    memcpy(skin.map, map, sizeof(map));

    player.name = p_attr.name;
    player.loc = getTopLocation(5);
    player.hitbox = hitbox;
    player.health = 100;
    player.damage = 1;
    player.offset[0] = 0.0;
    player.offset[1] = 0.0;
    player.skin = skin;
    player.deathEvent - onPlayerDeath;
    spawnEntity(&player);

    basic_sword.category = WEAPON;
    basic_sword.type = SMALL_SWORD;
    basic_sword.value = 10;
    basic_armor.category = ARMORY;
    basic_armor.type = HOOD_CAPE;
    basic_armor.value = 5;
    addItem(basic_sword);
    addItem(basic_armor);

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

void addItem(GItem item) {
    for (int i=0; i<10; i++) {
        GItem *slot = &inv.items[i];
        
        if (!slot->valid) {
            slot->valid = true;
            slot->type = item.type;
            slot->category = item.category;
            slot->equip = item.equip;
            slot->value = item.value;

            if (item.equip)
                inv.equipment[item.category] = slot;
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

void attack(Entity* entity) {
    int i = 0;
    Entity *victim;
    Vector e_loc;

    e_loc[0] = entity->loc.pos[0];
    e_loc[1] = entity->loc.pos[1];

    while (i < MAX_ENTITY) {
        victim = getEntityByID(i++);        

        if (victim != NULL) {
            if (strcmp(victim->name, entity->name) == 0)
                continue;

            float dist = distance(victim->loc.pos, e_loc);
            GItem *item = inv.equipment[WEAPON];
            float damage;
            const float crit_dist = 2.0;
            const float crit_mul = 2.0;
            
            if (dist > 3.0)
                continue;

            if (item == NULL) {
                damage = entity->damage;
            } else {
                damage = item->value;
            }

            if (crit_dist > dist) {
                damage *= (1 - crit_mul) / crit_dist * dist + crit_mul;
            }

            damage += damage * (p_attr.strength / 100);
            victim->health -= floor(damage);
            break;
        }
    }
}

void doTick(int key) {
    if (!inGame)
        return;
    
    if (getStage() != VOID) {
        int id = 0;
        Entity* iter = getEntityByID(id++);

        if (iter != NULL) {
            updateControl(key, iter);
        }

        while (iter != NULL) {
            if (iter->health <= 0) {
                iter->deathEvent(iter);
                break;
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

static void onPlayerDeath(Entity* entity) {
    entity->health = 100;
    generateLevel(LOBBY);
}