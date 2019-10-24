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

void spawnEntity(Entity* e) {
    Entity* iter = NULL;
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
            e->next = NULL;

            if (first) {
                entity = e;
                first = false;
            } else {
                before->next = e;
            }
            break;
        }
        else if (first) {
            first = false;
            iter = iter->next;
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
                free(iter);
                before->next = NULL;
            }
            return 1;
        }
        else if (first) {
            first = false;
            iter = iter->next;
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

    if (!iter || !iter->valid) {
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
    } while (iter && iter->valid);

    return empty;
}

Location getSpawnLocation(int x_pos) {
    Location loc;
    loc.pos[0] = (float) x_pos;
    loc.spd[0] = 0.0;
    loc.spd[1] = 0.0;
    
    if (tiles != NULL) {
        for (int y=0; y<level_height; y++) {
            if (tiles[y][x_pos] == TILE_AIR) {
                loc.pos[1] = y + 3;
                break;
            }
        }
    }

    return loc;
}

void updateEntities() {
    Entity *iter = entity;

    while (iter != NULL && iter->valid) {
        updatePhysic(iter);
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