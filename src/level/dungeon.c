#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header/level.h"

static Entity* monster;

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

        monster->loc = getTopLocation(50);
        monster->hitbox = hitbox;
        monster->name = "Magika";
        monster->health = 50;
        monster->damage = 5;
        monster->offset[0] = 0.0;
        monster->offset[1] = 0.0;
        monster->skin = skin;
        monster->deathEvent = onMonsterDeath;
        spawnEntity(monster);
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
    exit.color = GREEN;
    exit.tile = PORTAL_1;
    exit.dest = LOBBY;

    assignPortal(exit);
    setTileAt(10, 6, PORTAL_1);
    setTileAt(11, 6, PORTAL_1);
    setTileAt(12, 6, PORTAL_1);
    despawnEntity(entity->name);
    free(monster);
    monster = NULL;
}