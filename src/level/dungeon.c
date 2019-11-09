#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header/level.h"
#include "header/screen.h"
#include "header/game.h"
#include "header/data.h"

#define MAX_REWARD 3
#define MAX_MONSTER 3

DungeonType dungeon;
Entity monster[MAX_MONSTER];
int alive;

static void spawnMonsters();
static void onMonsterDeath(Entity*);

void generateDungeon() {
    spawnMonsters();
}

void destructDungeon() {
    for (int i = 0; i < MAX_MONSTER; i++) {
        if (despawnEntity(monster[i].id)) {
            alive--;
        }
    }
}

static void spawnMonsters() {
    int id, vol, xPos = 60;

    alive = 0;

    for (id = 0; id < MAX_MONSTER; id++) {
        AABB hitbox;
        Texture skin;
        bool map[9][9] = {false};
        int type = dungeonData[dungeon][D_MONSTER_1 + id];
        
        monster[id].id = -1;

        if (type < 0) continue;
        
        skin.color = monsterData[type][M_COLOR];
        vol = monsterData[type][M_VOLUME];
        hitbox.centre[0] = 0.0;
        hitbox.centre[1] = 0.0;
        hitbox.radius[0] = monsterData[type][M_VOLUME] - 1.0;
        hitbox.radius[1] = monsterData[type][M_VOLUME] - 1.0;

        if (vol <= 1) {
            map[4][4] = true;
            monster[id].offset[1] = 1.0;
        }
        else if (vol == 2) {
            for (int i = 3; i <= 5; i++)
                for (int j = 3; j <= 5; j++) {
                    map[i][j] = true;
                }
        }
        else {
            for (int i = 2; i <= 6; i++)
                for (int j = 2; j <= 6; j++) {
                    map[i][j] = true;
                }
        }
        memcpy(skin.map, map, sizeof(map));

        monster[id].type[0] = MONSTER;
        monster[id].type[1] = type;
        monster[id].loc = getTopLocation(xPos);
        monster[id].target = getEntityByID(0);
        monster[id].hitbox = hitbox;
        monster[id].name = getMonsterName(type);
        monster[id].mp = 0;
        monster[id].health = monsterData[type][M_MAX_HEALTH];
        monster[id].damage = monsterData[type][M_DAMAGE];
        monster[id].offset[0] = 0.0;
        monster[id].offset[1] = 0.0;
        monster[id].skin = skin;
        monster[id].deathEvent = onMonsterDeath;
        spawnEntity(&monster[id]);

        xPos += 10;
        alive++;
    }
}

static void onMonsterDeath(Entity* entity) {
    Portal exit;
    WINDOW* win;
    char reward_msg[100];
    int cnt = 0;
    bool full = false;
    int exp;

    despawnEntity(entity->id);

    if (--alive > 0) return;

    exp = dungeonData[dungeon][D_EXP];
    exit.color = GREEN;
    exit.tile = PORTAL_1;
    exit.dest = LOBBY;
    memset(reward_msg, '\0', strlen(reward_msg));

    for (int i = 0; i < MAX_REWARD; i++) {
        int type = dungeonData[dungeon][D_REWARD_1 + i];
        const char* str;

        if (type < 0) continue;

        str = getItemName(type);
        
        if (addItem(type)) {
            strcat(reward_msg, str);
            strcat(reward_msg, ", ");
        } else {
            full = true;
        }
    }

    if (reward_msg[0] != '\0') {
        reward_msg[strlen(reward_msg) - 2] = '\0';
    }

    addExp(exp);

    assignPortal(exit);
    setTileAt(10, 6, PORTAL_1);
    setTileAt(11, 6, PORTAL_1);
    setTileAt(12, 6, PORTAL_1);

    setPromptMode(DIALOGUE_PROMPT);
    win = menu_win(getPromptMenu());
    mvwprintw(win, 1, 3, "%s Clear! You defeated every monsters.", getDungeonName(dungeon));
    mvwprintw(win, 3, 3, "* EXP Gained: %d", exp);
    mvwprintw(win, 4, 3, "* Reward: %s", reward_msg);

    if (full)
        mvwprintw(win, 5, 3, "> Some reward is lost because your inventory is full!");

    wrefresh(win);
}