// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdlib.h>
#include <string.h>
#include "header/level.h"
#include "header/screen.h"
#include "header/physic.h"

int level_width = 0, level_height = 0, spawn_x = 5, portal_i = -1;
Stage stage = LOBBY;
static Portal portal_arr[30];
static Tile **tiles = NULL;
static Entity *entity = NULL;

/**
 * Spawn an entity.
 * Player must be assigned before any other entities.
 **/
void spawnEntity(Entity* e) {
    Entity* iter = entity;
    Entity dummy = {false};
    bool first = true;

    if (entity == NULL) {
        iter = entity = &dummy;
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
 * Returns a pointer of entity
 * If you want to retrieve the first entry, pass NULL to name.
 * The function returns NULL if failed to query
 **/
Entity* getEntity(const char* name) {
    Entity *iter = entity;

    if (!iter || !iter->valid) {
        return NULL;
    }
    else if (name == NULL) {
        return iter;
    }

    do {
        if (strcmp(name, iter->name) == 0) {
            return iter;
        }

        iter = iter->next;
    } while (iter && iter->valid);

    return NULL;
}

Location getSpawnLocation() {
    Location loc;
    loc.pos[0] = (float) spawn_x;
    loc.spd[0] = 0.0;
    loc.spd[1] = 0.0;
    
    if (tiles != NULL) {
        for (int y=0; y<level_height; y++) {
            if (tiles[y][spawn_x] == AIR) {
                loc.pos[1] = y + 3;
                break;
            }
        }
    }

    return loc;
}

void assignPortal(Portal instance) {
    portal_arr[++portal_i] = instance;
}

/**
 * Returns a warp matching the given tile.
 * (NULL if not found)
 **/
Portal *getPortal(Tile tile) {
    for (int i=0; i<=portal_i; i++) {
        Portal iter = portal_arr[i];

        if (tile == iter.tile) {
            return &portal_arr[i];
        }
    }

    return NULL;
}

void generateLevel() {
    level_width = 200;
    level_height = 50;

    tiles = calloc(level_height, sizeof(Tile *));

    for (int r=0; r<level_height; r++) {
        tiles[r] = (Tile *) calloc(level_width, sizeof(Tile));
    }

    for (int y=0; y<level_height; y++) {
        for (int x=0; x<level_width; x++) {
            if (y > 5) {
                tiles[y][x] = AIR;
            }
            else {
                tiles[y][x] = BLOCK;
            }
        }
    }

    switch (stage) {
        case LOBBY:
            generateLobby(tiles);
            break;
    }
}

void destructLevel() {
    if (tiles != NULL) {
        free(tiles);
        tiles = NULL;
    }

    if (portal_i > -1) {
        for (int i=0; i<10; i++) {
            Portal instance = {false};
            portal_arr[i] = instance;
        }
        portal_i = -1;
    }
}

void setStage(Stage _stage) {
    stage = _stage;
}

Tile getTileAt(int x, int y) {
    if (tiles == NULL || x >= level_width || x < 0 || y >= level_height || y < 0) {
        return AIR;
    }

    return tiles[y][x];
}