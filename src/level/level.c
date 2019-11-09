// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdlib.h>
#include <string.h>
#include "header/level.h"
#include "header/screen.h"
#include "header/physic.h"

int level_width = 0, level_height = 0;
static Entity* entityList = NULL; // Entity Linked-array (Game data)
static Portal* portalList = NULL; // Portal Linked-array (Game data)
static Stage stage = VOID;
static Tile **tiles = NULL; // Map tiles 2D array (Not a structure)

/**
 * Spawn an entity.
 * Player must be assigned before any other entities.
 * Returns: entity ID
 **/
int spawnEntity(Entity e) {
    Bias bias = {false, true, 0, 0, 0};
    Entity *iter = entityList;
    Entity *prev = NULL;
    static int id = 0;

    while (iter) {
        prev = iter;
        iter = iter->next;
    }

    iter = malloc(sizeof(Entity));

    if (!entityList) {
        entityList = iter;
    }

    e.id = id;
    e.bias = bias;
    e.next = NULL;
    e.prev = prev;
    *iter = e;

    if (prev) {
        prev->next = iter;
    }

    return id++;
}

bool despawnEntity(int id) {
    Entity *e = getEntityByID(id);

    if (e) {
        Entity *prev = e->prev;
        Entity *next = e->next;

        e->id = -1;

        if (prev)
            prev->next = next;

        if (next)
            next->prev = prev;

        return true;
    }
    return false;
}

/**
 * Returns the pointer of entity matching the ID.
 * The NULL pointer as result is possible.
 * Player always occupy the first entry (id 0)
 **/
Entity* getEntityByID(int id) {
    Entity *iter = entityList;

    while (iter) {
        if (iter->id == id) {
            return iter;
        }

        iter = iter->next;
    }

    return NULL;
}

/* Returns location at top of the blocks below */
Location getTopLocation(int pos_x) {
    Location loc;
    loc.pos[0] = (float) pos_x;
    loc.spd[0] = 0.0;
    loc.spd[1] = 0.0;
    
    if (tiles) {
        for (int y = level_height - 1; y > 0; y--) {
            if (tiles[y][pos_x] == BLOCK) {
                loc.pos[1] = y + 3;
                break;
            }
        }
    }

    return loc;
}

void assignPortal(Portal instance) {
    Portal *iter = portalList;
    Portal *prev = NULL;

    while (iter) {
        prev = iter;
        iter = iter->next;
    }

    iter = malloc(sizeof(Portal));

    if (!portalList) {
        portalList = iter;
    }

    instance.next = NULL;
    instance.prev = prev;
    *iter = instance;
    
    if (prev) {
        prev->next = iter;
    }
}

/**
 * Returns a warp matching the given tile.
 * (NULL if not found)
 **/
Portal *getPortal(Tile tile) {
    Portal *iter = portalList;

    while (iter) {
        if (iter->tile == tile) {
            return iter;
        }

        iter = iter->next;
    }

    return NULL;
}

void destructLevel() {
    Portal *p = portalList;

    if (stage == DUNGEON)
        destructDungeon();

    if (tiles) {
        free(tiles);
        tiles = NULL;
    }

    while (p) {
        Portal *prev = p;
        p = p->next;

        free(prev);
    }

    portalList = NULL;
}

/**
 * Generate the level with a new stage.
 **/
void generateLevel(Stage _stage) {
    if (stage != VOID) {
        destructLevel();
    }
    stage = _stage;
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
        case DUNGEON:
            generateDungeon(tiles);
            break;
    }
}

Stage getStage() {
    return stage;
}

char* getStageName(Stage _stage) {
    switch (_stage) {
        case VOID:
            return "Void";
        case LOBBY:
            return "Lobby";
        case SHOP:
            return "Shop";
        case DUNGEON:
            return "Dungeon";
    }

    return NULL;
}

Tile getTileAt(int x, int y) {
    if (tiles == NULL || x >= level_width || x < 0 || y >= level_height || y < 0)
        return AIR;

    return tiles[y][x];
}

void setTileAt(int x, int y, Tile tile) {
    if (tiles == NULL || x >= level_width || x < 0 || y >= level_height || y < 0)
        return;
    
    tiles[y][x] = tile;
}