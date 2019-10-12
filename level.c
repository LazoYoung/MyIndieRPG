// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdlib.h>
#include <string.h>
#include "level.h"
#include "screen.h"

int level_width = 0, level_height = 0;
char **tiles = NULL;
static Entity *entity = NULL;

void spawnEntity(const char* name, Location loc) {
    Entity* iter;
    bool first = true;

    if (entity == NULL) {
        iter = entity = malloc(sizeof(Entity));
    }
    else {
        iter = entity;
    }
    
    while (1) {
        Entity* before = iter;

        if (!first) {
            iter = iter->next;
        }

        if (iter == NULL || !iter->valid) {
            Entity *instance = malloc(sizeof(Entity));
            instance->valid = true;
            instance->name = name;
            instance->loc = loc;
            instance->next = NULL;

            if (first) {
                entity = instance;
                first = false;
            } else {
                before->next = instance;
            }
            break;
        }
    }
}

int despawnEntity(const char* name) {
    Entity *iter = entity;    
    bool first = true;

    if (entity == NULL) {
        return 0;
    }

    while (1) {
        Entity *before = iter;

        if (!first) {
            iter = iter->next;
        }

        if (iter == NULL || !iter->valid) {
            break;
        }

        if (strcmp(name, iter->name) == 0) {
            if (first) {
                free(entity);
                entity = NULL;
                first = false;
            }
            else {
                free(before->next);
                before->next = NULL;
            }
            return 1;
        }
    }

    return 0;
}

/**
 * Returns a copy of entity status.
 * If you want to retrieve first entry, pass NULL to name.
 **/
Entity getEntity(const char* name) {
    Entity *iter = entity;
    Entity empty = {false};

    if (!iter->valid) {
        return empty;
    }
    else if (name == NULL) {
        return *iter;
    }

    do {
        if (strcmp(name, iter->name) == 0) {
            return *iter;
        }

        iter = iter->next;
    } while (iter != NULL && iter->valid);

    return empty;
}

Location getSpawnLocation(int x_pos) {
    Location loc;
    loc.pos_x = x_pos;
    loc.vec_x = 0.0;
    loc.vec_y = 0.0;
    
    if (tiles != NULL) {
        for (int y=0; y<level_height; y++) {
            if (tiles[y][x_pos] == TILE_AIR) {
                loc.pos_y = y + 3;
                break;
            }
        }
    }

    return loc;
}

void updateEntities() {
    Entity *iter = entity;

    while (iter != NULL && iter->valid) {
        // TODO Do physics

        iter = iter->next;
    }
}

void generateLevel() {
    level_width = 200;
    level_height = 50;
    int ground = 20;
    tiles = calloc(level_height, sizeof(char *));

    for (int r=0; r<level_height; r++) {
        tiles[r] = (char *) calloc(level_width, sizeof(char));
    }

    for (int y=0; y<level_height; y++) {
        for (int x=0; x<level_width; x++) {
            if (y > 5) {
                tiles[y][x] = TILE_AIR;
            }
            else {
                tiles[y][x] = TILE_BLOCK;
            }
        }
    }
}

void destructLevel() {
    free(tiles);
    tiles = NULL;
}