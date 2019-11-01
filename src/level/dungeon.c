#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header/level.h"
#include "header/screen.h"
#include "header/game.h"

#define MAX_REWARD 3

static Entity* monster;
static int exp;
static int reward[MAX_REWARD];

static void onMonsterDeath(Entity*);

void generateDungeon() {
    if (getStage() == DUNGEON_TEST) {
        monster = malloc(sizeof(Entity));
        AABB hitbox = {{0.0, 0.0}, {1.0, 1.0}};
        Texture skin;
        bool map[9][9] = {false};

        skin.color = RED;
        map[3][3] = map[3][4] = map[3][5] = true;
        map[4][3] = map[4][4] = map[4][5] = true;
        map[5][3] = map[5][4] = map[5][5] = true;
        memcpy(skin.map, map, sizeof(map));

        monster->type = MONSTER;
        monster->loc = getTopLocation(50);
        monster->target = getEntityByID(0);
        monster->hitbox = hitbox;
        monster->name = "Magika";
        monster->health = 50;
        monster->damage = 5;
        monster->absorb = 0;
        monster->agility = -50;
        monster->strength = 0;
        monster->offset[0] = 0.0;
        monster->offset[1] = 0.0;
        monster->skin = skin;
        monster->deathEvent = onMonsterDeath;
        spawnEntity(monster);

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

static void onMonsterDeath(Entity* entity) {
    Portal exit;
    WINDOW* win;
    char name[30];
    char rewards[100];
    int cnt = 0;
    
    exit.color = GREEN;
    exit.tile = PORTAL_1;
    exit.dest = LOBBY;
    strcpy(name, entity->name);
    memset(rewards, '\0', strlen(rewards));

    for (int i = 0; i < MAX_REWARD; i++) {
        int type = reward[i];

        if (type > -1) {
            const char* str = getItemName(type);

            if (str != NULL)
                strcat(rewards, str);
            
            strcat(rewards, ", ");
            addItem(type);
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
    wrefresh(win);
}