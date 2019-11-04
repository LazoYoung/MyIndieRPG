#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header/level.h"
#include "header/screen.h"
#include "header/game.h"
#include "header/data.h"

#define MAX_REWARD 3

DungeonType dungeon;
static Entity* monster;
static int exp;
static ItemType reward[MAX_REWARD];

static void spawnMonster(MonsterType);
static void onMonsterDeath(Entity*);

void generateDungeon() {
    if (dungeon == DUNGEON_1) {
        spawnMonster(GOLEM);

        exp = 100;
        reward[0] = BRONZE_SWORD;
        reward[1] = STEEL_BLADE;
        reward[2] = -1;
    }
}

void destructDungeon() {
    if (monster != NULL) {
        despawnEntity(monster->name);
        free(monster);
        monster = NULL;
    }
}

static void spawnMonster(MonsterType type) {
    monster = malloc(sizeof(Entity));
    AABB hitbox = {{0.0, 0.0}, {1.0, 1.0}};
    Texture skin;
    bool map[9][9] = {false};

    skin.color = RED;
    map[3][3] = map[3][4] = map[3][5] = true;
    map[4][3] = map[4][4] = map[4][5] = true;
    map[5][3] = map[5][4] = map[5][5] = true;
    memcpy(skin.map, map, sizeof(map));

    monster->type[0] = MONSTER;
    monster->type[1] = type;
    monster->loc = getTopLocation(50);
    monster->target = getEntityByID(0);
    monster->hitbox = hitbox;
    monster->name = getMonsterName(type);
    monster->mp = 0;
    monster->health = monsterAttr[type][M_MAX_HEALTH];
    monster->damage = monsterAttr[type][M_DAMAGE];
    monster->offset[0] = 0.0;
    monster->offset[1] = 0.0;
    monster->skin = skin;
    monster->deathEvent = onMonsterDeath;
    spawnEntity(monster);
}

static void onMonsterDeath(Entity* entity) {
    Portal exit;
    WINDOW* win;
    char name[30];
    char rewards[100];
    int cnt = 0;
    bool full = false;
    
    exit.color = GREEN;
    exit.tile = PORTAL_1;
    exit.dest = LOBBY;
    strcpy(name, entity->name);
    memset(rewards, '\0', strlen(rewards));

    for (int i = 0; i < MAX_REWARD; i++) {
        int type = reward[i];

        if (type > -1) {
            const char* str = getItemName(type);
            
            if (addItem(type)) {
                strcat(rewards, str);
                strcat(rewards, ", ");
            } else {
                full = true;
            }
        }
    }

    if (rewards[0] != '\0') {
        rewards[strlen(rewards) - 2] = '\0';
    }

    addExp(exp);

    assignPortal(exit);
    setTileAt(10, 6, PORTAL_1);
    setTileAt(11, 6, PORTAL_1);
    setTileAt(12, 6, PORTAL_1);
    despawnEntity(name);
    free(monster);
    monster = NULL;

    setPromptMode(DIALOGUE_PROMPT);
    win = menu_win(getPromptMenu());
    mvwprintw(win, 1, 3, "Dungeon Clear! You've defeated %s.", name);
    mvwprintw(win, 3, 3, "EXP Gained: %d", exp);
    mvwprintw(win, 4, 3, "Reward: %s", rewards);

    if (full)
        mvwprintw(win, 5, 3, "> Some reward is lost because your inventory is full!");

    wrefresh(win);
}