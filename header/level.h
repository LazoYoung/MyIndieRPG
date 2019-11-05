#ifndef FILE_LEVEL_INCLUDED
#define FILE_LEVEL_INCLUDED
#define MAX_ENTITY 4

#include <stdbool.h>
#include "game-struct.h"
#include "data.h"

extern int level_width, level_height;
extern DungeonType dungeon;

extern void spawnEntity(Entity*);
extern bool despawnEntity(int id);
extern Entity* getEntity(const char*);
extern Entity* getEntityByID(int);
extern Location getTopLocation(int);
extern void assignPortal(Portal);
extern Portal* getPortal(Tile);
extern void generateLevel(Stage);
extern void destructLevel();
extern Stage getStage();
extern char* getStageName(Stage);
extern char* getDungeonName(DungeonType type);
extern Tile getTileAt(int, int);
extern void setTileAt(int x, int y, Tile tile);

// lobby.c
extern void generateLobby();

// dungeon.c
extern void generateDungeon();
extern void destructDungeon();

#endif