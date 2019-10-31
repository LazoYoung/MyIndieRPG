#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header/level.h"

static Entity* monster_p;

void generateDungeon(Tile** tiles) {
    if (getStage() == DUNGEON_TEST) {
        static Entity monster;
        AABB hitbox = {{0.0, 0.0}, {1.0, 1.0}};
        Texture skin;
        bool map[9][9] = {false};

        skin.color = RED;
        map[3][3] = map[3][4] = map[3][5] = true;
        map[4][3] = map[4][4] = map[4][5] = true;
        map[5][3] = map[5][4] = map[5][5] = true;
        memcpy(skin.map, map, sizeof(map));

        monster_p = &monster;
        monster.loc = getTopLocation(50);
        monster.hitbox = hitbox;
        monster.name = "Bullsht";
        monster.health = 50;
        monster.damage = 5;
        monster.offset[0] = 0.0;
        monster.offset[1] = 0.0;
        monster.skin = skin;
        spawnEntity(monster_p);
    }
}

void destructDungeon() {
    despawnEntity(monster_p->name);
    free(monster_p);
}